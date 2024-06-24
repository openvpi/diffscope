#ifndef SCRIPT_MANAGER_SCRIPTMANAGERPLUGIN_H
#define SCRIPT_MANAGER_SCRIPTMANAGERPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace ScriptManager::Internal {

    class ScriptManagerPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.OpenVPI.DiffScope.Plugin" FILE "plugin.json")
    public:
        explicit ScriptManagerPlugin();
        ~ScriptManagerPlugin() override;

        bool initialize(const QStringList &arguments, QString *errorString) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

}

#endif // SCRIPT_MANAGER_SCRIPTMANAGERPLUGIN_H
