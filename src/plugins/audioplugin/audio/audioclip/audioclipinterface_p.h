#ifndef AUDIO_AUDIOCLIPINTERFACE_P_H
#define AUDIO_AUDIOCLIPINTERFACE_P_H

#include <memory>

#include <audioplugin/audioclipinterface.h>

namespace Audio {
    class AudioClipInterfacePrivate {
        Q_DECLARE_PUBLIC(AudioClipInterface)
    public:
        AudioClipInterface *q_ptr;

        QDspx::AudioClipEntity *entity;
        TrackInterface *track;

        std::unique_ptr<talcs::PositionableMixerAudioSource> clipControlMixer;
        talcs::PositionableMixerAudioSource *clipMixer;
        talcs::PositionableAudioSource *clipAudioSource;
    };
}

#endif // AUDIO_AUDIOCLIPINTERFACE_P_H
