#ifndef AUDIO_TRACKINTERFACE_P_H
#define AUDIO_TRACKINTERFACE_P_H

#include <memory>

#include <QMutex>

#include <audioplugin/trackinterface.h>

namespace QDspx {
    class AudioClipEntity;
}

namespace Audio {

    class TrackInterfacePrivate {
        Q_DECLARE_PUBLIC(TrackInterface);
    public:
        TrackInterface *q_ptr;

        QDspx::TrackEntity *entity;
        talcs::DspxTrackContext *trackContext;

        AudioContextInterface *audioContextInterface;
        QHash<QDspx::AudioClipEntity *, AudioClipInterface *> clips;

        void handleEntityGainChanged(double gainDecibel) const;
        void handleEntityPanChanged(double pan) const;
        void handleEntityMuteChanged(bool isMuted) const;
        void handleEntitySoloChanged(bool isSolo) const;

        void handleAudioClipInserted(int id, QDspx::AudioClipEntity *audioClipEntity);
        void handleAudioClipAboutToRemove(int id, QDspx::AudioClipEntity *audioClipEntity);
    };

}

#endif // AUDIO_TRACKINTERFACE_P_H
