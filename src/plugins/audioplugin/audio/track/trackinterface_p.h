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

        void handleGainChanged(double gainDecibel) const;
        void handlePanChanged(double pan) const;
        void handleMuteChanged(bool isMuted) const;
        void handleSoloChanged(bool isSolo) const;

        void handleAudioClipInserted(int id, QDspx::AudioClipEntity *audioClipEntity);
        void handleAudioClipAboutToRemove(int id, QDspx::AudioClipEntity *audioClipEntity);
    };

}

#endif // AUDIO_TRACKINTERFACE_P_H
