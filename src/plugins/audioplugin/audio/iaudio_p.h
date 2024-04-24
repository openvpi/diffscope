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
        QList<const QMetaObject *> outputSystemAddOnClasses;
        QList<const QMetaObject *> audioContextAddOnClasses;
    };
}

#endif // AUDIO_IAUDIO_P_H
