#ifndef AUDIO_TRACKINTERFACE_P_H
#define AUDIO_TRACKINTERFACE_P_H

#include <memory>

#include <audioplugin/trackinterface.h>

namespace QDspx {
    class ClipEntity;
}

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

        void handleEntityGainChange(double gainDecibel) const;
        void handleEntityPanChange(double pan) const;
        void handleEntityMuteChange(bool isMuted) const;
        void handleEntitySoloChange(bool isSolo) const;

        void handleClipInserted(QDspx::ClipEntity *clipEntity);
        void handleClipAboutToRemove(QDspx::ClipEntity *clipEntity);
    };

}

#endif // AUDIO_TRACKINTERFACE_P_H
