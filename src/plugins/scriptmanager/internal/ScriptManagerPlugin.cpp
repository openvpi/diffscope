#include "ScriptManagerPlugin.h"

#include <extensionsystem/pluginspec.h>

#include <QMWidgets/qmdecoratorv2.h>

#include <CoreApi/iloader.h>

#include <idecoreFramework/initroutine.h>

#include <coreplugin/icore.h>

#ifdef SCRIPT_MANAGER_TEST
#    include <scriptmanager/internal/TestModelWrapper.h>
#endif

namespace ScriptManager::Internal {
    ScriptManagerPlugin::ScriptManagerPlugin() {
    }
    ScriptManagerPlugin::~ScriptManagerPlugin() {
    }
    bool ScriptManagerPlugin::initialize(const QStringList &arguments, QString *errorString) {
        qIDec->addTranslationPath(pluginSpec()->location() + QStringLiteral("/translations"));

        qDebug() << "ScriptManager: initializing";
        auto splash = Core::InitRoutine::splash();
        splash->showMessage(tr("Initializing script manager..."));
        qobject_cast<QObject *>(this);
#ifdef SCRIPT_MANAGER_TEST
        auto iLoader = Core::ILoader::instance();
        iLoader->addObject("debug.InteractiveDebug", new TestModelWrapper);
#endif
        return true;
    }
    void ScriptManagerPlugin::extensionsInitialized() {
    }
    bool ScriptManagerPlugin::delayedInitialize() {
        return IPlugin::delayedInitialize();
    }
}