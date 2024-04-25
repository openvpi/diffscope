#ifndef AUDIO_AUDIOPLUGIN_H
#define AUDIO_AUDIOPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Audio {

    class AudioPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.OpenVPI.DiffScope.Plugin" FILE "plugin.json")
    public:
        AudioPlugin();
        ~AudioPlugin() override;

        bool initialize(const QStringList &arguments, QString *errorString) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;

        QObject *remoteCommand(const QStringList &, const QString &, const QStringList &) override;
    };

}

#endif // AUDIO_AUDIOPLUGIN_H
