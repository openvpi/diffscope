#ifndef AUDIO_OUTPUTSYSTEMINTERFACE_P_H
#define AUDIO_OUTPUTSYSTEMINTERFACE_P_H

#include <audioplugin/outputsysteminterface.h>

namespace Audio {

    namespace Internal {
        class AbstractOutputSystem;
    }

    class IOutputSystemAddOn;

    class OutputSystemInterfacePrivate {
        Q_DECLARE_PUBLIC(OutputSystemInterface)
    public:
        OutputSystemInterface *q_ptr;

        Internal::AbstractOutputSystem *abstractOutputSystem;
        bool isVST;

        void init(Internal::AbstractOutputSystem *abstractOutputSystem, bool isVST);
    };

}

#endif // AUDIO_OUTPUTSYSTEMINTERFACE_P_H
