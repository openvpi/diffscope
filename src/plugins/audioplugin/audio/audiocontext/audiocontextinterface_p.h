#ifndef AUDIO_AUDIOCONTEXTINTERFACE_P_H
#define AUDIO_AUDIOCONTEXTINTERFACE_P_H

#include <audioplugin/audiocontextinterface.h>

#include <audioplugin/itrackaddon.h>
#include <audioplugin/trackinterface.h>
#include <audioplugin/iaudioclipaddon.h>
#include <audioplugin/audioclipinterface.h>

namespace Audio {

    class IAudioContextAddOn;

    class AudioContextInterfacePrivate {
        Q_DECLARE_PUBLIC(AudioContextInterface)
    public:
        AudioContextInterface *q_ptr;

        ProjectAddOn *projectAddOn;
        Core::IExecutiveRegistry<TrackInterface> trackRegistry;
        Core::IExecutiveRegistry<AudioClipInterface> audioClipRegistry;
        QHash<QDspx::TrackEntity *, TrackInterface *> tracks;

        void init(ProjectAddOn *projectAddOn);

    };
}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_P_H
