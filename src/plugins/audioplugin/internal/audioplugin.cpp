#include "audioplugin.h"

namespace Audio {

    AudioPlugin::AudioPlugin() {
    }
    AudioPlugin::~AudioPlugin() = default;
    bool AudioPlugin::initialize(const QStringList &arguments, QString *errorString) {
        return true;
    }
    void AudioPlugin::extensionsInitialized() {
    }
    bool AudioPlugin::delayedInitialize() {
        return false;
    }
    QObject *AudioPlugin::remoteCommand(const QStringList &options, const QString &workingDirectory, const QStringList &args) {
        if (options.contains("-vst")) {

        } else {

        }
        return nullptr;
    }

} // Audio