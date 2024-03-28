#ifndef AUDIO_VSTADDON_H
#define AUDIO_VSTADDON_H

#include <CoreApi/iwindowaddon.h>

namespace Audio {

    class VSTAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit VSTAddOn(QObject *parent = nullptr);
        ~VSTAddOn();

        void initialize() override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

} // Audio

#endif // AUDIO_VSTADDON_H
