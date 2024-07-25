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
        talcs::DspxAudioClipContext *audioClipContext;

        TrackInterface *trackInterface;

        void handleGainChanged(double gainDecibel) const;
        void handlePanChanged(double pan) const;
        void handleMuteChanged(bool isMute) const;

        void handleStartChanged(int start) const;
        void handleClipStartChanged(int clipStart) const;
        void handleClipLenChanged(int clipLen) const;
        void handlePathChanged(const QString &path) const;
    };
}

#endif // AUDIO_AUDIOCLIPINTERFACE_P_H
