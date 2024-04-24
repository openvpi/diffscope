#ifndef AUDIO_AUDIOCONTEXTINTERFACE_P_H
#define AUDIO_AUDIOCONTEXTINTERFACE_P_H

#include <memory>

#include <audioplugin/audiocontextinterface.h>
#include <audioplugin/internal/addonloader.h>

namespace Audio {

    class IAudioContextAddOn;

    class AudioContextInterfacePrivate {
        Q_DECLARE_PUBLIC(AudioContextInterface)
    public:
        AudioContextInterface *q_ptr;

        ProjectAddOn *projectAddOn;
        std::unique_ptr<AddOnLoader<IAudioContextAddOn>> addOnLoader;

        void init(ProjectAddOn *projectAddOn);
        void initializeAddOns();

    };
}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_P_H
