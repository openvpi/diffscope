#ifndef AUDIO_AUDIOCONTEXTINTERFACE_P_H
#define AUDIO_AUDIOCONTEXTINTERFACE_P_H

#include <audioplugin/audiocontextinterface.h>

#include <TalcsCore/BufferingAudioSource.h>

#include <QMCore/qmchronomap.h>

#include <audioplugin/itrackaddon.h>
#include <audioplugin/trackinterface.h>
#include <audioplugin/iaudioclipaddon.h>
#include <audioplugin/audioclipinterface.h>

namespace talcs {
    class DspxProjectContext;
}

namespace Audio {

    class IAudioContextAddOn;

    class AudioContextInterfacePrivate {
        Q_DECLARE_PUBLIC(AudioContextInterface)
    public:
        AudioContextInterface *q_ptr;

        Internal::ProjectAddOn *projectAddOn;
        Core::IExecutiveRegistry<TrackInterface> trackRegistry;
        Core::IExecutiveRegistry<AudioClipInterface> audioClipRegistry;

        talcs::DspxProjectContext *projectContext;

        QHash<QDspx::TrackEntity *, TrackInterface *> tracks;
        QHash<QDspx::AudioClipEntity *, AudioClipInterface *> clips;

        void init(Internal::ProjectAddOn *projectAddOn);

        void handleGainChanged(double gainDecibel) const;
        void handlePanChanged(double pan) const;
        void handleMuteChanged(bool isMuted) const;

        void handleTrackInserted(int index, QDspx::TrackEntity *trackEntity);
        void handleTrackAboutToRemove(int index, QDspx::TrackEntity *trackEntity);
        void handleTrackMoved(int index, int count, int dest) const;

    };
}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_P_H
