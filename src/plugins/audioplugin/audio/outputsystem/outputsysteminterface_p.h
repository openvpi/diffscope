#ifndef AUDIO_OUTPUTSYSTEMINTERFACE_P_H
#define AUDIO_OUTPUTSYSTEMINTERFACE_P_H

#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/internal/addonloader.h>

namespace Audio {

    class AbstractOutputSystem;
    class IOutputSystemAddOn;

    class OutputSystemInterfacePrivate {
        Q_DECLARE_PUBLIC(OutputSystemInterface)
    public:
        OutputSystemInterface *q_ptr;

        AbstractOutputSystem *abstractOutputSystem;
        bool isVST;
        QScopedPointer<AddOnLoader<IOutputSystemAddOn>> addOnLoader;

        void initializeAddOns();
    };

}

#endif // AUDIO_OUTPUTSYSTEMINTERFACE_P_H
