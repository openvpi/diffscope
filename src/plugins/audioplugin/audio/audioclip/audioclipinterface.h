#ifndef AUDIO_AUDIOCLIPINTERFACE_H
#define AUDIO_AUDIOCLIPINTERFACE_H

#include <CoreApi/iexecutive.h>

#include <audioplugin/audioglobal.h>

namespace QDspx {
    class AudioClipEntity;
}

namespace talcs {
    class PositionableMixerAudioSource;
    class PositionableAudioSource;
    class DspxAudioClipContext;
}

namespace Audio {

    class IAudioClipAddOn;

    class TrackInterface;

    class AudioClipInterfacePrivate;

    class AUDIO_EXPORT AudioClipInterface : public Core::IExecutive {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AudioClipInterface)
        friend class Core::IExecutiveRegistry<AudioClipInterface>;
        using AddOnType = IAudioClipAddOn;

    public:
        ~AudioClipInterface() override;

        QDspx::AudioClipEntity *entity() const;
        TrackInterface *trackInterface() const;

        talcs::PositionableMixerAudioSource *clipMixer() const;
        talcs::PositionableAudioSource *contentSource() const;

    protected:
        explicit AudioClipInterface(QDspx::AudioClipEntity *entity, TrackInterface *trackInterface, talcs::DspxAudioClipContext *audioClipContext);
        explicit AudioClipInterface(AudioClipInterfacePrivate &d, QObject *parent);

    private:
        QScopedPointer<AudioClipInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_AUDIOCLIPINTERFACE_H
