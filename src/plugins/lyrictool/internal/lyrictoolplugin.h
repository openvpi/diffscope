#ifndef LYRICTOOLPLUGIN_H
#define LYRICTOOLPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace LyricTool::Internal {

    class LyricToolPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.OpenVPI.DiffScope.Plugin" FILE "plugin.json")
    public:
        LyricToolPlugin();
        ~LyricToolPlugin();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

}

#endif // LYRICTOOLPLUGIN_H
