#ifndef DEBUG_DEBUGPLUGIN_H
#define DEBUG_DEBUGPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Debug::Internal {

    class DebugPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.OpenVPI.DiffScope.Plugin" FILE "plugin.json")
    public:
        explicit DebugPlugin();
        ~DebugPlugin() override;

        bool initialize(const QStringList &arguments, QString *errorString) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

}

#endif // DEBUG_DEBUGPLUGIN_H
