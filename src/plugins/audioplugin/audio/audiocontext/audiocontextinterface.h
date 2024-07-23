#ifndef AUDIO_AUDIOCONTEXTINTERFACE_H
#define AUDIO_AUDIOCONTEXTINTERFACE_H

#include <QObject>

#include <audioplugin/audioglobal.h>

namespace talcs {
    class MixerAudioSource;
    class TransportAudioSource;
    class PositionableMixerAudioSource;
}

namespace Core {
    class IProjectWindow;
    template <class T> class IExecutiveRegistry;
}

namespace QDspx {
    class TrackEntity;
    class AudioClipEntity;
}

namespace Audio {

    namespace Internal {
        class ProjectAddOn;
    }

    class TrackInterfacePrivate;
    class AudioClipInterfacePrivate;

    class OutputSystemInterface;
    class TrackInterface;
    class AudioClipInterface;

    class AudioContextInterfacePrivate;

    class AUDIO_EXPORT AudioContextInterface : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AudioContextInterface)
        friend class Internal::ProjectAddOn;
        friend class TrackInterfacePrivate;
        friend class AudioClipInterfacePrivate;
    public:
        ~AudioContextInterface() override;

        talcs::MixerAudioSource *preMixer() const;
        talcs::TransportAudioSource *transport() const;
        talcs::PositionableMixerAudioSource *postMixer() const;
        talcs::PositionableMixerAudioSource *masterTrackMixer() const;

        Core::IProjectWindow *windowHandle() const;
        static AudioContextInterface *get(Core::IProjectWindow *win);

        OutputSystemInterface *outputSystemInterface() const;

        Core::IExecutiveRegistry<TrackInterface> *trackRegistry() const;
        Core::IExecutiveRegistry<AudioClipInterface> *audioClipRegistry() const;

        QList<TrackInterface *> tracks() const;

        TrackInterface *getTrackInterfaceFromEntity(QDspx::TrackEntity *entity) const;
        AudioClipInterface *getAudioClipInterfaceFromEntity(QDspx::AudioClipEntity *entity) const;

    private:
        explicit AudioContextInterface(QObject *parent = nullptr);
        QScopedPointer<AudioContextInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_H
