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
    class AbstractAudioFormatIO;
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
        QHash<QDspx::TrackEntity *, TrackInterface *> tracks;

        void init(Internal::ProjectAddOn *projectAddOn);

        void handleTrackInserted(QDspx::TrackEntity *trackEntity);
        void handleTrackAboutToRemove(QDspx::TrackEntity *trackEntity);

    };
}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_P_H
