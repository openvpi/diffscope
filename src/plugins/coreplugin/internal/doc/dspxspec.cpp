#include "dspxspec.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QSharedPointer>
#include <QTimer>

#include <CoreApi/documentsystem.h>
#include <CoreApi/private/documentspec_p.h>

#include <dspxmodel/dspxconst.h>

#include "dspxdocument.h"

namespace Core {

    class DspxSpecPrivate : public QObject, public DocumentSpecPrivate {
        Q_DECLARE_PUBLIC(DspxSpec)
    public:
        DspxSpecPrivate();
        ~DspxSpecPrivate();

        void init();
    };

    DspxSpecPrivate::DspxSpecPrivate() {
    }

    DspxSpecPrivate::~DspxSpecPrivate() {
    }

    void DspxSpecPrivate::init() {
    }

    static DspxSpec *m_instance = nullptr;

    static QString specId() {
        static const auto id = QStringLiteral("org.ChorusKit.dspx");
        return id;
    }

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec(*new DspxSpecPrivate(), specId(), parent) {
        m_instance = this;

        Q_D(DspxSpec);
        d->init();

        setIcon(QIcon(":/images/dspx.png"));
        setDisplayName([]() { return tr("OpenVPI DiffSinger Editor"); });
        setDescription([]() { return tr("Built-in"); });
    }

    DspxSpec::~DspxSpec() {
        m_instance = nullptr;
    }

    DspxSpec *DspxSpec::instance() {
        return m_instance;
    }

    QVersionNumber DspxSpec::currentVersion() {
        static QVersionNumber ver = QVersionNumber::fromString(QDspx::Version);
        return ver;
    }

    QString DspxSpec::logBaseDir() {
        return DocumentSystem::logBaseDir() + "/" + specId();
    }

    QStringList DspxSpec::supportedExtensions() const {
        return {
            QStringLiteral("dspx"),
        };
    }

    QString DspxSpec::filter() const {
        return QStringLiteral("%1(%2)").arg(tr("DiffScope Project Files"),
                                            QStringLiteral("*.dspx"));
    }

    bool DspxSpec::open(const QString &fileName, QWidget *parent) {
        if (DocumentSpec::open(fileName, parent))
            return true;

        auto doc = new DspxDocument();
        if (!doc->open(fileName)) {
            QMessageBox::critical(parent, tr("File Error"), doc->errorMessage());
            delete doc;
            return false;
        }

        // IWindow::create<IProjectWindow>(doc);
        return true;
    }

}
