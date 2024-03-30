#include "sdlaudiodevicedisplaynamehelper.h"

#include <QHash>

namespace Audio {
    QString SDLAudioDeviceDisplayNameHelper::getDisplayNameOf(const QString &deviceName) {
        static QHash<QString, QString> map = {
            {"pipewire",    "Pipewire"                   },
            {"pulseaudio",  "PulseAudio"                 },
            {"alsa",        "ALSA PCM audio"             },
            {"jack",        "JACK Audio Connection Kit"  },
            {"sndio",       "OpenBSD sndio"              },
            {"netbsd",      "NetBSD audio"               },
            {"dsp",         "OSS /dev/dsp standard audio"},
            {"qsa",         "QNX QSA Audio"              },
            {"audio",       "UNIX /dev/audio interface"  },
            {"arts",        "Analog RealTime Synthesizer"},
            {"esd",         "Enlightened Sound Daemon"   },
            {"nacl",        "SDL NaCl Audio Driver"      },
            {"nas",         "Network Audio System"       },
            {"wasapi",      "WASAPI"                     },
            {"directsound", "DirectSound"                },
            {"winmm",       "Windows Waveform Audio"     },
            {"paud",        "AIX Paudio"                 },
            {"haiku",       "Haiku BSoundPlayer"         },
            {"coreaudio",   "CoreAudio"                  },
            {"disk",        "direct-to-disk audio"       },
            {"dummy",       "SDL dummy audio driver"     },
            {"fusionsound", "FusionSound"                },
            {"AAudio",      "AAudio audio driver"        },
            {"openslES",    "opensl ES audio driver"     },
            {"android",     "SDL Android audio driver"   },
            {"ps2",         "PS2 audio driver"           },
            {"psp",         "PSP audio driver"           },
            {"vita",        "VITA audio driver"          },
            {"n3ds",        "SDL N3DS audio driver"      },
            {"emscripten",  "SDL emscripten audio driver"},
            {"DART",        "OS/2 DART"                  },
        };
        return map.value(deviceName, deviceName);
    }
} // Audio