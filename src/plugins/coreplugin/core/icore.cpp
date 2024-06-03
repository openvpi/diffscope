#include "icore.h"

#include <csignal>

#include "QStyleFactory"
#include <QApplication>
#include <QChildEvent>
#include <QMessageBox>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QGridLayout>

#include <extensionsystem/pluginmanager.h>

#include <QMWidgets/qmview.h>

#include <CoreApi/iloader.h>
#include <CoreApi/private/icorebase_p.h>

#include <idecoreFramework/settingsdialog.h>
#include <idecoreFramework/plugindialog.h>

#include "dspxspec.h"
#include "ihomewindow.h"
#include "appextra.h"

namespace Core {

    class ICorePrivate : ICoreBasePrivate {
        Q_DECLARE_PUBLIC(ICore)
    public:
        ICorePrivate() {
        }

        void init() {
            Q_Q(ICore);
            actionManager = new ActionManager(q);
        }

        ActionManager *actionManager;
    };

    ICore *ICore::instance() {
        return static_cast<ICore *>(ICoreBase::instance());
    }

    int ICore::showSettingsDialog(const QString &id, QWidget *parent) {
        static SettingsDialog *dlg = nullptr;

        if (dlg) {
            if (!id.isEmpty())
                dlg->selectPage(id);
            return -1;
        }

        int code;
        {
            SettingsDialog dlg2(parent);
            dlg = &dlg2;
            if (!id.isEmpty())
                dlg2.selectPage(id);
            code = dlg2.exec();
            dlg = nullptr;
        }

        return code;
    }

    void ICore::showPluginsDialog(QWidget *parent) {
        PluginDialog dlg(parent);
        dlg.exec();
    }

    void ICore::showHome() {
        auto inst = IHomeWindow::instance();
        if (inst) {
            QMView::raiseWindow(inst->window());
            return;
        }
        IHomeWindowRegistry::instance()->create();
    }

    ActionManager *ICore::actionManager() const {
        Q_D(const ICore);
        return d->actionManager;
    }

    void ICore::newFile() const {
    }

    bool ICore::openFile(const QString &fileName, QWidget *parent) const {
        auto docMgr = ICore::instance()->documentSystem();
        if (fileName.isEmpty()) {
            return docMgr->openFileBrowse(parent, DspxSpec::instance());
        }
        return DspxSpec::instance()->open(fileName, parent);
    }

    ICore::ICore(QObject *parent) : ICore(*new ICorePrivate(), parent) {
    }

    ICore::~ICore() {
    }

    ICore::ICore(ICorePrivate &d, QObject *parent) : ICoreBase(d, parent) {
        d.init();
    }


}