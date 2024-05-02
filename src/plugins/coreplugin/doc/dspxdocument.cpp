#include "dspxdocument.h"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVersionNumber>
#include <QReadWriteLock>

#include <opendspx/qdspxmodel.h>

#include <substate/node.h>
#include <substate/model.h>
#include <substate/fsengine.h>
#include <qsubstate/private/qsubstateglobal_p.h>

#include <CoreApi/private/idocument_p.h>

#include <dspxmodel/dspxconst.h>

#include "dspxspec.h"
#include "icore.h"
#include "appextra.h"

namespace Core {

    static int m_untitledIndex = 0;

    static inline std::filesystem::path qstr2path(const QString &s) {
        return
#ifdef Q_OS_WINDOWS
            s.toStdWString()
#else
            s.toStdString()
#endif
                ;
    }

    class DspxDocumentPrivate : public IDocumentPrivate, Substate::Subscriber {
        Q_DECLARE_PUBLIC(DspxDocument)
    public:
        DspxDocumentPrivate();
        virtual ~DspxDocumentPrivate();

        void init();

        bool opened;
        bool watched;
        int initialStep;
        int savedStep;
        bool accepted;

        QString logDir;
        QString usedLogDir;

        QDspx::ModelEntity *objModel;
        Substate::Model *dataModel;

        inline void changeToOpen();
        inline void changeToSaved();
        inline void changeToUnsaved();
        inline void changeToWatched();
        inline void unshiftToRecent();

        bool create(const QDspxModel &model);
        bool read(const QByteArray &data);
        QByteArray save() const;
        bool makeNew();
        bool recover();

        void notified(Substate::Notification *n);
    };

    DspxDocumentPrivate::DspxDocumentPrivate() {
        opened = false;
        watched = false;
        initialStep = 0;
        savedStep = -1;
        accepted = false;

        objModel = nullptr;
        dataModel = nullptr;
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
        if (!objModel)
            return;

        delete objModel;
        delete dataModel;

        if (accepted) {
            QDir dir(usedLogDir);
            if (dir.isAbsolute()) {
                dir.removeRecursively();
            }
        }
    }

    void DspxDocumentPrivate::init() {
        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addDocument(q, true);
    }

    inline void DspxDocumentPrivate::changeToOpen() {
        Q_Q(DspxDocument);
        opened = true;

        Q_EMIT q->changed();
    }

    inline void DspxDocumentPrivate::changeToSaved() {
        Q_Q(DspxDocument);

        savedStep = dataModel->currentStep();
        Q_EMIT q->changed();
    }

    inline void DspxDocumentPrivate::changeToUnsaved() {
        Q_Q(DspxDocument);

        savedStep = -1;
        Q_EMIT q->changed();
    }

    void DspxDocumentPrivate::changeToWatched() {
        if (!watched) {
            return;
        }
        watched = true;

        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addDocument(q);
    }

    void DspxDocumentPrivate::unshiftToRecent() {
        Q_Q(DspxDocument);
        ICore::instance()->documentSystem()->addRecentFile(q->filePath());
    }

    static void dataToModel(const QDspxModel &model, QDspx::ModelEntity *modelEntity) {
        // TODO
    }

    static void modelToData(QDspxModel &model, const QDspx::ModelEntity *modelEntity) {
        // TODO
    }

    bool DspxDocumentPrivate::create(const QDspxModel &model) {
        Q_Q(DspxDocument);

        if (objModel) {
            Q_EMIT q->docReset();

            delete objModel;
            delete dataModel;

            objModel = nullptr;
            dataModel = nullptr;
        }

        {
            // Remove previous dir
            QDir dir(usedLogDir);
            if (dir.isAbsolute() && !dir.removeRecursively()) {
                errMsg = DspxDocument::tr("%1: Failed to remove old directory.").arg(usedLogDir);
                return false;
            }
            usedLogDir.clear();
        }

        {
            QDir dir(logDir);

            // In case the log dir is empty
            if (!dir.isAbsolute()) {
                errMsg = DspxDocument::tr("Directory not set.");
                return false;
            }
            // Remove old dir
            if (dir.exists() && !dir.removeRecursively()) {
                errMsg = DspxDocument::tr("%1: Failed to remove old directory.").arg(usedLogDir);
                return false;
            }
            // Create current dir
            if (!dir.mkpath(logDir)) {
                errMsg = DspxDocument::tr("%1: Failed to create directory.").arg(usedLogDir);
                return false;
            }
        }

        // Initialize filesystem engine
#if 1
        auto engine = new Substate::FileSystemEngine();
        if (!engine->start(qstr2path(logDir))) {
            errMsg = DspxDocument::tr("Failed to initialize transaction engine.");
            delete engine;
            return false;
        }
        dataModel = new Substate::Model(engine);
#else
        dataModel = new Substate::Model();
#endif
        dataModel->addSubscriber(this);

        objModel = new QDspx::ModelEntity();
        dataToModel(model, objModel);

        dataModel->beginTransaction();
        dataModel->setRoot(objModel->internalData());
        dataModel->commitTransaction({
            {"open_dspx", "true"}
        });

        usedLogDir = logDir;
        savedStep = dataModel->currentStep();
        return true;
    }

    bool DspxDocumentPrivate::read(const QByteArray &data) {
        Q_Q(DspxDocument);

        QDspxModel model;
        auto loadResult = model.loadData(data);
        switch (loadResult.type) {
            case QDspx::Result::InvalidFormat:
                errMsg = DspxDocument::tr("Invalid format!");
                return false;
            case QDspx::Result::Success:
                break;
            default:
                errMsg = DspxDocument::tr("Unknown error!");
                return false;
        }

        // Get version
        if (model.version.isEmpty()) {
            errMsg = DspxDocument::tr("Missing version field!");
            return false;
        }

        // Check version
        QVersionNumber version = QVersionNumber::fromString(model.version);
        if (version > DspxSpec::currentVersion()) {
            errMsg =
                DspxDocument::tr("The specified file version(%1) is too high").arg(model.version);
            return false;
        }
        return create(model);
    }

    QByteArray DspxDocumentPrivate::save() const {
        QDspxModel model;
        modelToData(model, objModel);
        return model.saveData();
    }

    bool DspxDocumentPrivate::makeNew() {
        QDspxModel model;
        model.version = QDspx::Version;
        model.content.global.name = QStringLiteral("New Project");
        return create(model);
    }

    bool DspxDocumentPrivate::recover() {
        Q_Q(DspxDocument);

#if 1
        auto engine = new Substate::FileSystemEngine();
        if (!engine->recover(qstr2path(logDir))) {
            errMsg = DspxDocument::tr("Failed to recover from journal.");
            delete engine;
            return false;
        }

        dataModel = new Substate::Model(engine);
#else
        dataModel = new Substate::Model();
#endif
        dataModel->addSubscriber(this);

        objModel = new QDspx::ModelEntity(dataModel->root(), false);

        dataModel->beginTransaction();
        dataModel->setRoot(objModel->internalData());
        dataModel->commitTransaction({
            {"open_dspx", "true"}
        });
        return true;
    }

    void DspxDocumentPrivate::notified(Substate::Notification *n) {
        Q_Q(DspxDocument);
        switch (n->type()) {
            case Substate::Notification::StepChange: {
                Q_EMIT q->changed();
                break;
            }
            default:
                break;
        }
    }

    DspxDocument::DspxDocument(QObject *parent)
        : IDocument(*new DspxDocumentPrivate(), QStringLiteral("org.ChorusKit.dspx"), parent) {
        Q_D(DspxDocument);
        d->init();
    }

    DspxDocument::~DspxDocument() = default;

    DspxDocument::DataModel::State DspxDocument::DataModel::state() const {
        return static_cast<State>(d->dataModel->state());
    }
    QVariantHash DspxDocument::DataModel::stepMessage(int index) const {
        QVariantHash msg;
        const auto &message = d->dataModel->stepMessage(index);
        msg.reserve(int(message.size()));
        for (const auto &it : message) {
            msg.insert(QString::fromStdString(it.first), it.second.value<QVariant>());
        }
        return msg;
    }
    int DspxDocument::DataModel::minimumStep() const {
        return qMax(d->dataModel->minimumStep(), d->initialStep);
    }
    int DspxDocument::DataModel::maximumStep() const {
        return d->dataModel->maximumStep();
    }
    int DspxDocument::DataModel::currentStep() const {
        return d->dataModel->currentStep();
    }
    void DspxDocument::DataModel::beginTransaction() {
        d->dataModel->beginTransaction();
    }
    void DspxDocument::DataModel::abortTransaction() {
        d->dataModel->abortTransaction();
    }
    void DspxDocument::DataModel::commitTransaction(const QVariantHash &message) {
        Substate::Engine::StepMessage msg;
        msg.reserve(message.size());
        for (auto it = message.begin(); it != message.end(); ++it) {
            msg.insert(
                std::make_pair(it.key().toStdString(), Substate::Variant::fromValue(it.value())));
        }
        d->dataModel->commitTransaction(msg);
    }
    void DspxDocument::DataModel::undo() {
        d->dataModel->undo();
    }
    void DspxDocument::DataModel::redo() {
        d->dataModel->redo();
    }
    QDspx::ModelEntity *DspxDocument::DataModel::model() const {
        return d->objModel;
    }

    DspxDocument::DataModel DspxDocument::dataModel() const {
        Q_D(const DspxDocument);
        DataModel model;
        model.d = d;
        return model;
    }

    QString DspxDocument::dataDirectory() const {
        Q_D(const DspxDocument);
        return d->logDir;
    }

    void DspxDocument::setDataDirectory(const QString &dir) {
        Q_D(DspxDocument);
        d->logDir = dir;
    }

    bool DspxDocument::accepted() const {
        return false;
    }

    void DspxDocument::setAccepted(bool accepted) {
        Q_D(DspxDocument);
        d->accepted = accepted;
    }

    bool DspxDocument::open(const QString &fileName) {
        Q_D(DspxDocument);

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            setErrorMessage(tr("Failed to open file!"));
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        if (!d->read(data)) {
            return false;
        }

        setFilePath(fileName);
        d->changeToOpen();
        d->changeToWatched();
        d->unshiftToRecent();
        return true;
    }

    bool DspxDocument::openRawData(const QString &suggestFileName, const QByteArray &data) {
        Q_D(DspxDocument);

        if (!d->read(data)) {
            return false;
        }

        QFileInfo info(suggestFileName);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();
        return true;
    }

    bool DspxDocument::makeNew(const QString &suggestFileName) {
        Q_D(DspxDocument);

        if (!d->makeNew()) {
            return false;
        }
        QFileInfo info(suggestFileName.isEmpty()
                           ? QStringLiteral("Untitled-%1").arg(QString::number(++m_untitledIndex))
                           : suggestFileName);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();
        return true;
    }

    bool DspxDocument::recover(const QString &filename) {
        Q_D(DspxDocument);

        if (!d->recover()) {
            return false;
        }

        QFileInfo info(filename);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();
        return true;
    }

    bool DspxDocument::save(const QString &fileName) {
        Q_D(DspxDocument);

        QByteArray data = d->save();

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            setErrorMessage(tr("Failed to create file!"));
            return false;
        }
        file.write(data);
        file.close();

        setFilePath(fileName);
        d->unshiftToRecent();
        d->changeToSaved();
        d->changeToWatched();
        return true;
    }

    void DspxDocument::changeSavedState(bool saved) {
        Q_D(DspxDocument);
        if (saved)
            d->changeToSaved();
        else
            d->changeToUnsaved();
    }

    QByteArray DspxDocument::rawData() const {
        Q_D(const DspxDocument);
        return d->save();
    }

    bool DspxDocument::isOpened() const {
        Q_D(const DspxDocument);
        return d->opened;
    }

    bool DspxDocument::isFile() const {
        Q_D(const DspxDocument);
        return d->watched;
    }

    QString DspxDocument::defaultPath() const {
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    QString DspxDocument::suggestedFileName() const {
        return QStringLiteral("Untitled.dspx");
    }

    bool DspxDocument::isModified() const {
        Q_D(const DspxDocument);
        if (!d->opened)
            return false;
        return d->savedStep != d->dataModel->currentStep();
    }

    void DspxDocument::close() {
        if (!QFileInfo(filePath()).isFile()) {
            ICore::instance()->documentSystem()->removeRecentFile(filePath());
        }
        IDocument::close();
    }

    IDocument::ReloadBehavior DspxDocument::reloadBehavior(ChangeTrigger state,
                                                           ChangeType type) const {
        switch (type) {
            case TypePermissions:
                return BehaviorSilent;
            case TypeContents:
                if (state == TriggerInternal && !isModified())
                    return BehaviorSilent;
                break;
            default:
                break;
        }
        return BehaviorAsk;
    }

    bool DspxDocument::reload(ReloadFlag flag, ChangeType type) {
        Q_D(DspxDocument);
        Q_UNUSED(type)
        // Content changed
        if (flag == FlagReload) {
            d->savedStep = -1;
        }
        Q_EMIT changed();
        return true;
    }

}
