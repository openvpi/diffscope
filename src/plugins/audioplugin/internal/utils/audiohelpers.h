#ifndef AUDIO_AUDIOHELPERS_H
#define AUDIO_AUDIOHELPERS_H

#include <QtGlobal>

namespace Audio::Internal {

    class AudioHelpers {
    public:
        static constexpr qint64 msecToSample(int msec, double sampleRate) {
            return static_cast<qint64>(sampleRate / 1000.0 * msec);
        }
    };

}

#endif // AUDIO_AUDIOHELPERS_H
