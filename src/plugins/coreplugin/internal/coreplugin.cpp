#include "coreplugin.h"

#include <QApplication>
#include <QDirIterator>
#include <QFileOpenEvent>
#include <QLoggingCategory>
#include <QSplashScreen>
#include <QThread>

#include <extensionsystem/pluginspec.h>
#include <extensionsystem/pluginmanager.h>

#include <QMWidgets/qmdecoratorv2.h>
#include <QMWidgets/qmview.h>

#include <CoreApi/iloader.h>

#include <appshared/initroutine.h>

#include "ihomewindow.h"

#include "dspxspec.h"
#include "homeaddon.h"
#include "actionconfigurepage.h"
#include "appearancetoppage.h"
#include "displaypage.h"

namespace Core::Internal {

    static ICore *icore = nullptr;

    static int openFileFromCommand(QString workingDir, const QStringList &args, IWindow *iWin) {
        int cnt = 0;

        if (workingDir.isEmpty())
            workingDir = QDir::currentPath();

        for (const auto &arg : qAsConst(args)) {
            QFileInfo info(arg);
            if (info.isRelative()) {
                info.setFile(workingDir + QStringLiteral("/") + arg);
            }

            if (!info.isFile()) {
                continue;
            }

            auto spec = icore->documentSystem()->supportedDocType(info.completeSuffix());
            if (!spec)
                continue;
            if (spec->open(info.canonicalFilePath(), iWin ? iWin->window() : nullptr)) {
                cnt++;
            }
        }

        return cnt;
    }

    static void waitSplash(QWidget *w) {
        // Get splash screen handle
        auto ir = AppShared::InitRoutine::instance();
        ir->splash->finish(w);
        delete ir;
    }

    CorePlugin::CorePlugin() {
    }

    CorePlugin::~CorePlugin() {
    }

    bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));
        qIDec->addThemePath(pluginSpec()->location() + QStringLiteral("/themes"));

        auto ir = AppShared::InitRoutine::instance();
        ir->splash->showMessage(tr("Initializing core plugin..."));

        // Init ICore instance
        icore = new ICore(this);

        // Add basic actions
        // auto actionMgr = icore->actionSystem();

        // if (actionMgr->loadContexts(":/core_actions.xml").isEmpty()) {
        //     *errorMessage = tr("Failed to load action configuration!");
        //     return false;
        // }

        // Add basic windows and add-ons
        auto winMgr = icore->windowSystem();
        winMgr->addAddOn(QStringLiteral("home"), &HomeAddOn::staticMetaObject);
        // winMgr->addAddOn("project", &ProjectWindowAddOn::staticMetaObject);

        // Add setting panels
        auto sc = icore->settingCatalog();
        {
            auto appearance = new AppearanceTopPage();

            auto display = new DisplayPage();
            auto actionConfigure = new ActionConfigurePage();

            appearance->addPage(display);
            appearance->addPage(actionConfigure);

            sc->addPage(appearance);
        }

        // Add document types
        auto docMgr = icore->documentSystem();
        {
            auto dspxSpex = new DspxSpec();
            docMgr->addDocType(dspxSpex);
            docMgr->setPreferredDocTypeId(QStringLiteral("dspx"), dspxSpex->id());
        }

        // QLoggingCategory::setFilterRules("qt.gui.shortcutmap=true");
        // qApp->setWindowIcon(QIcon(":/svg/app/diffsinger.svg"));

        // Handle FileOpenEvent
        qApp->installEventFilter(this);

        return true;
    }

    void CorePlugin::extensionsInitialized() {
        // Theme fallback
        if (!qIDec->themes().contains(qIDec->theme())) {
            qIDec->setTheme(QStringLiteral("Visual Studio Code - Dark"));
        }
    }

    bool CorePlugin::delayedInitialize() {
        auto ir = AppShared::InitRoutine::instance();
        ir->splash->showMessage(tr("Initializing user interface..."));

        if (ir->entry) {
            waitSplash(ir->entry());
            return false;
        }

        if (openFileFromCommand({}, ExtensionSystem::PluginManager::arguments(), nullptr) > 0) {
            waitSplash(icore->windowSystem()->firstWindow()->window());
        } else {
            waitSplash(IWindow::create<IHomeWindow>()->window());
        }

        return false;
    }

    QObject *CorePlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                       const QStringList &args) {
        auto firstHandle = icore->windowSystem()->firstWindow();
        int cnt = openFileFromCommand(workingDirectory, args, firstHandle);
        if (firstHandle && cnt == 0) {
            QMView::raiseWindow(firstHandle->window());
        }
        return nullptr;
    }

    bool CorePlugin::eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::FileOpen) {
            openFileFromCommand({}, {static_cast<QFileOpenEvent *>(event)->file()},
                                icore->windowSystem()->firstWindow());
        }
        return QObject::eventFilter(obj, event);
    }

    // This scope is only to expose strings to Qt translation tool
    static void _qt_translation_CommandCategory() {
        QApplication::translate("Core::CommandCategory", "Create");
        QApplication::translate("Core::CommandCategory", "File");
        QApplication::translate("Core::CommandCategory", "Preferences");
        QApplication::translate("Core::CommandCategory", "Edit");
        QApplication::translate("Core::CommandCategory", "View");
        QApplication::translate("Core::CommandCategory", "Playback");
        QApplication::translate("Core::CommandCategory", "Help");
        QApplication::translate("Core::CommandCategory", "About");
        QApplication::translate("Core::CommandCategory", "Edit Mode");
    }

}
