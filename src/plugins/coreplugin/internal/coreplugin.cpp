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
#include <QMWidgets/qmappextension.h>

#include <CoreApi/iloader.h>
#include <CoreApi/private/iloader_p.h>
#include <CoreApi/actiondomain.h>

#include <idecoreFramework/initroutine.h>

#include "dspxspec.h"

// Windows
#include "ihomewindow.h"
#include "iprojectwindow.h"

// Addons
#include "homeaddon.h"
#include "projectaddon.h"

// Settings
#include <idecoreFramework/appearancepage.h>

#include "enviromenttoppage.h"
#include "actionlayoutspage.h"
#include "keymappage.h"
#include "menutoolbarpage.h"

extern const Core::ActionExtension *ckGetStaticActionExtension_core_actions();

CK_STATIC_ACTION_EXTENSION_GETTER(core_actions, getMyActionExtension);

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
        InitRoutine::splash()->finish(w);
    }

    CorePlugin::CorePlugin() {
    }

    CorePlugin::~CorePlugin() {
    }

    bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));
        qIDec->addThemePath(pluginSpec()->location() + QStringLiteral("/themes"));

        InitRoutine::splash()->showMessage(tr("Initializing core plugin..."));

        // Init ICore instance
        icore = new ICore(this);

        icore->actionManager()->addSystemShortcutsFamily("DiffScope", {});

        // Add basic actions
        auto domain = icore->actionManager()->domain();
        domain->addExtension(getMyActionExtension());

        // Add addons
        IHomeWindowRegistry::instance()->attach<HomeAddOn>();
        IProjectWindowRegistry::instance()->attach<ProjectAddOn>();

        // Add setting panels
        auto sc = icore->settingCatalog();
        {
            auto environmentTop = new EnvironmentTopPage();
            sc->addPage(environmentTop);
            {
                auto appearance = new AppearancePage();
                environmentTop->addPage(appearance);

                //                auto actionLayouts = new ActionLayoutsPage();
                //                environmentTop->addPage(actionLayouts);

                auto keymap = new KeymapPage();
                environmentTop->addPage(keymap);

                auto menuToolbar = new MenuToolbarPage();
                environmentTop->addPage(menuToolbar);
            }
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

        // Load actions
        auto actionMgr = icore->actionManager();
        std::ignore = actionMgr->loadLayouts();
        // std::ignore = actionMgr->saveLayouts();
        if (!actionMgr->loadIcons()) {
            actionMgr->saveIcons();
        }
        if (!actionMgr->loadShortcuts()) {
            actionMgr->saveShortcuts();
        }
    }

    bool CorePlugin::delayedInitialize() {
        InitRoutine::splash()->showMessage(tr("Initializing user interface..."));

        if (auto entry = InitRoutine::startEntry()) {
            waitSplash(entry());
            return false;
        }

        if (openFileFromCommand({}, ExtensionSystem::PluginManager::arguments(), nullptr) > 0) {
            waitSplash(icore->windowSystem()->firstWindow()->window());
        } else {
            waitSplash(IHomeWindowRegistry::instance()->create()->window());
        }

        //        {
        //            QFile file("D:\\actions.xml");
        //            file.open(QIODevice::WriteOnly);
        //            file.write(icore->domain()->saveLayouts());
        //        }
        //        {
        //            QFile file("D:\\keys.json");
        //            file.open(QIODevice::WriteOnly);
        //            file.write(icore->domain()->saveOverriddenShortcuts());
        //        }
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

}
