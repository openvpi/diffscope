#ifndef AUDIO_TRACKINTERFACE_P_H
#define AUDIO_TRACKINTERFACE_P_H

#include <memory>

#include <audioplugin/trackinterface.h>

namespace Audio {

    class TrackInterfacePrivate {
        Q_DECLARE_PUBLIC(TrackInterface);
    public:
        TrackInterface *q_ptr;

        QDspx::TrackEntity *entity;
        AudioContextInterface *context;

        std::unique_ptr<talcs::PositionableMixerAudioSource> trackControlMixer;
        talcs::PositionableMixerAudioSource *trackMixer;
        talcs::AudioSourceClipSeries *clipSeries;

        QHash<QDspx::AudioClipEntity *, AudioClipInterface *> clips;
    };

}

#endif // AUDIO_TRACKINTERFACE_P_H
