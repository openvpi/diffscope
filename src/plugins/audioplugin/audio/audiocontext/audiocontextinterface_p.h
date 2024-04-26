#ifndef AUDIO_AUDIOCONTEXTINTERFACE_P_H
#define AUDIO_AUDIOCONTEXTINTERFACE_P_H

#include <audioplugin/audiocontextinterface.h>

namespace Audio {

    class IAudioContextAddOn;

    class AudioContextInterfacePrivate {
        Q_DECLARE_PUBLIC(AudioContextInterface)
    public:
        AudioContextInterface *q_ptr;
        ProjectAddOn *projectAddOn;

        void init(ProjectAddOn *projectAddOn);

    };
}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_P_H
