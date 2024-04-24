#ifndef AUDIO_AUDIOCONTEXTINTERFACE_H
#define AUDIO_AUDIOCONTEXTINTERFACE_H

#include <QObject>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class MixerAudioSource;
    class TransportAudioSource;
    class PositionableMixerAudioSource;
    class PositionableAudioSource;
}

namespace Core {
    class IProjectWindow;
}

namespace Audio {

    class ProjectAddOn;

    class AudioContextInterfacePrivate;

    class AUDIO_EXPORT AudioContextInterface : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AudioContextInterface)
        friend class ProjectAddOn;
    public:
        ~AudioContextInterface() override;

        talcs::MixerAudioSource *preMixer() const;
        talcs::TransportAudioSource *transportAudioSource() const;
        talcs::PositionableMixerAudioSource *postMixer() const;
        talcs::PositionableMixerAudioSource *masterTrackMixer() const;

        talcs::PositionableAudioSource *substitutedSource() const;

        Core::IProjectWindow *windowHandle() const;

    private:
        explicit AudioContextInterface(QObject *parent = nullptr);
        QScopedPointer<AudioContextInterfacePrivate> d_ptr;

    };

} // Audio

#endif // AUDIO_AUDIOCONTEXTINTERFACE_H
