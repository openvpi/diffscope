#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <extensionsystem/iplugin.h>

#include <coreplugin/icore.h>

namespace Core::Internal {

    class CorePlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.OpenVPI.DiffScope.Plugin" FILE "plugin.json")
    public:
        CorePlugin();
        ~CorePlugin();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;

        QObject *remoteCommand(const QStringList &options, const QString &workingDirectory,
                               const QStringList &args) override;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

}

#endif // COREPLUGIN_H
