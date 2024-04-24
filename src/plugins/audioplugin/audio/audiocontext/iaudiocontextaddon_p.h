#ifndef AUDIO_IAUDIOCONTEXTADDON_P_H
#define AUDIO_IAUDIOCONTEXTADDON_P_H

#include <audioplugin/iaudiocontextaddon.h>

namespace Audio {

    class AudioContextInterface;

    class IAudioContextAddOnPrivate {
        Q_DECLARE_PUBLIC(IAudioContextAddOn)
    public:
        IAudioContextAddOn *q_ptr;
        AudioContextInterface *audioContextInterface;
    };
}

#endif // AUDIO_IAUDIOCONTEXTADDON_P_H
