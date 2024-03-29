#include "dspxdocument.h"

#include <CoreApi/private/idocument_p.h>

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVersionNumber>

#include "dspxspec.h"
#include "icore.h"


namespace Core {

    static int m_untitledIndex = 0;

    class DspxDocumentPrivate : public IDocumentPrivate {
        Q_DECLARE_PUBLIC(DspxDocument)
    public:
        DspxDocumentPrivate();
        virtual ~DspxDocumentPrivate();

        void init();

        bool opened;
        bool watched;
        int savedStep;

        void changeToOpen();
        void changeToSaved();
        void changeToWatched();

        void unshiftToRecent();

        bool checkNotOpened() const;

        bool readFile(const QByteArray &data);
        bool saveFile(QByteArray *data) const;
    };

    DspxDocumentPrivate::DspxDocumentPrivate() {
        opened = false;
        watched = false;
        savedStep = -1;
    }

    DspxDocumentPrivate::~DspxDocumentPrivate() {
    }

    void DspxDocumentPrivate::init() {
        Q_Q(DspxDocument);

        // model = new AceTreeModel(q);
        // history = new DspxHistory(this);

        ICore::instance()->documentSystem()->addDocument(q, true);
    }

    void DspxDocumentPrivate::changeToOpen() {
        Q_Q(DspxDocument);
        opened = true;

        // settings = new QSettings(QString("%1/desc.tmp.ini").arg(q->logDir()),
        // QSettings::IniFormat); settings->setValue("File/Path", q->filePath());
        // settings->setValue("File/Editor", q->id());

        // savedStep = model->currentStep();

        // QObject::connect(model, &AceTreeModel::modelChanged, this,
        //                  &DspxDocumentPrivate::_q_modelChanged);

        emit q->changed();
    }

    void DspxDocumentPrivate::changeToSaved() {
        Q_Q(DspxDocument);

        // if (mode == DspxDocument::FileMode) {
        //     if (!preferredDisplayName.isEmpty()) {
        //         q->setPreferredDisplayName({});
        //     }
        // }

        // settings->setValue("File/Path", q->filePath());
        // savedStep = model->currentStep();

        emit q->changed();
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

    bool DspxDocumentPrivate::checkNotOpened() const {
        if (!opened)
            return true;
        errMsg = DspxDocument::tr("File is opened");
        return false;
    }

    bool DspxDocumentPrivate::readFile(const QByteArray &data) {
        Q_Q(DspxDocument);

        // Parse json
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
            errMsg = DspxDocument::tr("Invalid file format!");
            return false;
        }

        auto obj = doc.object();
        QJsonValue value;

        // Get version
        value = obj.value("version");
        if (!value.isString()) {
            errMsg = DspxDocument::tr("Missing version field!");
            return false;
        }

        // Check version
        QString versionString = value.toString();
        QVersionNumber version = QVersionNumber::fromString(versionString);
        if (version > DspxSpec::currentVersion()) {
            errMsg =
                DspxDocument::tr("The specified file version(%1) is too high").arg(versionString);
            return false;
        }

        // Get workspace
        value = obj.value("workspace");
        // Check plugins
        if (value.isObject()) {
        }

        // Get content
        value = obj.value("content");
        if (!value.isObject()) {
            errMsg = DspxDocument::tr("Missing content field!");
            return false;
        }

        // auto content = new DspxContentEntity();
        // content->initialize();

        // if (!content->read(value.toObject())) {
        //     delete content;
        //     errMsg = DspxDocument::tr("Error occurred while parsing file!");
        //     return false;
        // }

        // this->content = content;

        // QDir logDir(q->logDir());
        // logDir.mkdir("model");

        // auto backend = new AceTreeJournalBackend();
        // backend->start(logDir.absoluteFilePath("model"));

        // auto model = new AceTreeModel(backend, q);
        // model->beginTransaction();
        // model->setRootItem(content->treeItem());
        // model->commitTransaction({
        //     {"open_dspx", "true"},
        // });
        // this->model = model;

        return true;
    }

    bool DspxDocumentPrivate::saveFile(QByteArray *data) const {
        // auto obj = content->write().toObject();
        // if (obj.isEmpty()) {
        //     errMsg = DspxDocument::tr("Error occurred while saving file!");
        //     return false;
        // }

        QJsonObject obj;
        QJsonObject docObj;

        // Write version
        docObj.insert("version", DspxSpec::currentVersion().toString());

        // Write workspace
        docObj.insert("workspace", QJsonObject()); // Workspace

        // Write content
        docObj.insert("content", obj);

        *data =
            QJsonDocument(docObj).toJson(QJsonDocument::Compact); // Disable indent to reduce size
        return true;
    }

    DspxDocument::DspxDocument(QObject *parent) : DspxDocument(*new DspxDocumentPrivate(), parent) {
    }

    DspxDocument::~DspxDocument() {
    }

    bool DspxDocument::open(const QString &fileName) {
        Q_D(DspxDocument);

        if (!d->checkNotOpened()) {
            return false;
        }

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            setErrorMessage(tr("Failed to open file!"));
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        if (!d->readFile(data)) {
            return false;
        }

        setFilePath(fileName);
        d->changeToOpen();
        d->changeToWatched();

        d->unshiftToRecent();

        return true;
    }

    bool DspxDocument::save(const QString &fileName) {
        Q_D(DspxDocument);

        QByteArray data;
        if (!d->saveFile(&data)) {
            return false;
        }

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            setErrorMessage(tr("Failed to create file!"));
            return false;
        }
        file.write(data);
        file.close();

        // if (d->mode == FileMode) {
        setFilePath(fileName);
        d->unshiftToRecent();
        d->changeToSaved();
        d->changeToWatched();
        // }

        return true;
    }

    bool DspxDocument::openRawData(const QString &suggestFileName, const QByteArray &data) {
        Q_D(DspxDocument);

        if (!d->checkNotOpened()) {
            return false;
        }
        if (!d->readFile(data)) {
            return false;
        }

        QFileInfo info(suggestFileName);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();

        return true;
    }

    bool DspxDocument::saveRawData(QByteArray *data) const {
        Q_D(const DspxDocument);
        return d->saveFile(data);
    }

    void DspxDocument::makeNew(const QString &suggestFileName) {
        Q_D(DspxDocument);

        if (d->opened) {
            return;
        }

        // Read binary log
        // {
        //     auto content = new DspxContentEntity();
        //     content->initialize();

        //     QDir dir(this->logDir());
        //     dir.mkdir("model");

        //     auto backend = new AceTreeJournalBackend();
        //     backend->start(dir.absoluteFilePath("model"));

        //     auto model = new AceTreeModel(backend, this);
        //     model->beginTransaction();
        //     model->setRootItem(content->treeItem());
        //     model->commitTransaction({
        //         {"open_dspx", "true"},
        //     });

        //     model->beginTransaction();
        //     model->setRootItem(content->treeItem());
        //     model->commitTransaction({
        //         {"open_dspx", "true"},
        //     });

        //     d->model = model;
        //     d->content = content;
        // }

        QFileInfo info(suggestFileName.isEmpty()
                           ? QStringLiteral("Untitled-%1").arg(QString::number(++m_untitledIndex))
                           : suggestFileName);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();
    }

    bool DspxDocument::recover(const QString &filename) {
        Q_D(DspxDocument);

        if (!d->checkNotOpened()) {
            return false;
        }

        // Read binary log
        // {
        //     QDir dir(this->logDir() + "/model");

        //     auto backend = new AceTreeJournalBackend();
        //     if (!backend->recover(dir.absolutePath())) {
        //         delete backend;
        //         setErrorMessage(tr("Fail to read log"));
        //         return false;
        //     }
        //     d->model = new AceTreeModel(backend, this);
        // }

        // auto content = new DspxContentEntity();
        // content->setup(d->model->rootItem());
        // d->content = content;

        QFileInfo info(filename);
        setFilePath(info.fileName());
        setPreferredDisplayName(info.baseName());
        d->changeToOpen();

        return true;
    }

    bool DspxDocument::isOpened() const {
        Q_D(const DspxDocument);
        return d->opened;
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

        // return d->savedStep != d->model->currentStep();
        return true;
    }

    void DspxDocument::close() {
        if (!QFileInfo(filePath()).isFile()) {
            ICore::instance()->documentSystem()->removeRecentFile(filePath());
        }
        IDocument::close();
    }

    IDocument::ReloadBehavior DspxDocument::reloadBehavior(IDocument::ChangeTrigger state,
                                                           IDocument::ChangeType type) const {
        Q_UNUSED(state)
        if (type == TypeRemoved) {
            return BehaviorAsk;
        }
        return BehaviorSilent;
    }

    bool DspxDocument::reload(IDocument::ReloadFlag flag, IDocument::ChangeType type) {
        Q_UNUSED(flag)
        Q_UNUSED(type)
        emit changed();
        return true;
    }

    DspxDocument::DspxDocument(DspxDocumentPrivate &d, QObject *parent)
        : IDocument(d, QStringLiteral("org.ChorusKit.dspx"), parent) {
        d.init();
    }

}
