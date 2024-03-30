#ifndef AUDIO_SDLAUDIODEVICEDISPLAYNAMEHELPER_H
#define AUDIO_SDLAUDIODEVICEDISPLAYNAMEHELPER_H

#include <QString>

namespace Audio {

    class SDLAudioDeviceDisplayNameHelper {
    public:
        static QString getDisplayNameOf(const QString &deviceName);
    };

} // Audio

#endif // AUDIO_SDLAUDIODEVICEDISPLAYNAMEHELPER_H
