#ifndef AUDIO_IOUTPUTSYSTEMADDON_P_H
#define AUDIO_IOUTPUTSYSTEMADDON_P_H

#include <audioplugin/ioutputsystemaddon.h>

namespace Audio {
    class IOutputSystemAddOnPrivate {
        Q_DECLARE_PUBLIC(IOutputSystemAddOn)
    public:
        IOutputSystemAddOn *q_ptr;
        OutputSystemInterface *outputSystemInterface;
    };
}

#endif // AUDIO_IOUTPUTSYSTEMADDON_P_H
