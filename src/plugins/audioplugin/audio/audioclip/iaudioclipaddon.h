#ifndef AUDIO_IAUDIOCLIPADDON_H
#define AUDIO_IAUDIOCLIPADDON_H

#include <CoreApi/iexecutive.h>

namespace Audio {

    class AudioClipInterface;

    class IAudioClipAddOn : public Core::IExecutiveAddOn{
        Q_OBJECT
        friend class Core::IExecutiveRegistry<AudioClipInterface>;
    public:
        ~IAudioClipAddOn() override;

        AudioClipInterface *audioClipInterface() const;

    protected:
        explicit IAudioClipAddOn(QObject *parent = nullptr);
    };

}

#endif // AUDIO_IAUDIOCLIPADDON_H
