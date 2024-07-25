#ifndef AUDIO_IAUDIO_P_H
#define AUDIO_IAUDIO_P_H

#include <audioplugin/iaudio.h>

namespace Audio {
    class IAudioPrivate {
        Q_DECLARE_PUBLIC(IAudio)
    public:
        IAudio *q_ptr;
        OutputSystemInterface *outputSystemInterface;
        OutputSystemInterface *vstOutputSystemInterface;
        talcs::FormatManager *formatManager;
    };
}

#endif // AUDIO_IAUDIO_P_H
