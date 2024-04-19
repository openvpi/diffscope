#ifndef AUDIO_PROJECTADDON_H
#define AUDIO_PROJECTADDON_H

#include <CoreApi/iwindowaddon.h>

namespace Audio {

    class ProjectAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        explicit ProjectAddOn(QObject *parent = nullptr);
        ~ProjectAddOn();

        void initialize() override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

} // Audio

#endif // AUDIO_PROJECTADDON_H
