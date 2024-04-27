#ifndef AUDIO_AUDIOCLIPINTERFACE_H
#define AUDIO_AUDIOCLIPINTERFACE_H

#include <CoreApi/iexecutive.h>

namespace QDspx {
    class AudioClipEntity;
}

namespace talcs {
    class PositionableMixerAudioSource;
    class PositionableAudioSource;
}

namespace Audio {

    class IAudioClipAddOn;

    class TrackInterface;

    class AudioClipInterfacePrivate;

    class AudioClipInterface : public Core::IExecutive {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AudioClipInterface)
        friend class Core::IExecutiveRegistry<AudioClipInterface>;
        using AddOnType = IAudioClipAddOn;

    public:
        ~AudioClipInterface() override;

        QDspx::AudioClipEntity *entity() const;
        TrackInterface *track() const;

        talcs::PositionableMixerAudioSource *clipMixer() const;
        talcs::PositionableAudioSource *clipAudioSource() const;

    protected:
        explicit AudioClipInterface(QDspx::AudioClipEntity *entity, TrackInterface *track, QObject *parent = nullptr);
        explicit AudioClipInterface(AudioClipInterfacePrivate &d, QObject *parent);

    private:
        QScopedPointer<AudioClipInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_AUDIOCLIPINTERFACE_H
