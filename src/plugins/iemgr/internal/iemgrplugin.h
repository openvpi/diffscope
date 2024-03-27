#ifndef IMPORTEXPORTMANAGER_H
#define IMPORTEXPORTMANAGER_H

#include <extensionsystem/iplugin.h>

namespace IEMgr ::Internal {

    class IEMgrPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.OpenVPI.DiffScope.Plugin" FILE "plugin.json")
    public:
        IEMgrPlugin();
        ~IEMgrPlugin();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

}

#endif // IMPORTEXPORTMANAGER_H
