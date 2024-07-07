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
    template <class T> class IExecutiveRegistry;
}

namespace QDspx {
    class TrackEntity;
}

namespace Audio {

    namespace Internal {
        class ProjectAddOn;
    }

    class OutputSystemInterface;

    class TrackInterface;

    class AudioClipInterface;

    class AudioContextInterfacePrivate;

    class AUDIO_EXPORT AudioContextInterface : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AudioContextInterface)
        friend class Internal::ProjectAddOn;

    public:
        ~AudioContextInterface() override;

        talcs::MixerAudioSource *preMixer() const;
        talcs::TransportAudioSource *transportAudioSource() const;
        talcs::PositionableMixerAudioSource *postMixer() const;
        talcs::PositionableMixerAudioSource *masterTrackMixer() const;

        Core::IProjectWindow *windowHandle() const;
        static AudioContextInterface *get(Core::IProjectWindow *win);

        OutputSystemInterface *outputSystem() const;

        Core::IExecutiveRegistry<TrackInterface> *trackRegistry() const;
        Core::IExecutiveRegistry<AudioClipInterface> *audioClipRegistry() const;

        QList<TrackInterface *> tracks() const;
        TrackInterface *getTrack(QDspx::TrackEntity *entity) const;

        talcs::PositionableAudioSource *getFormatSource(const QString &filename, const QVariant &userData, bool isInternal = true);

    private:
        explicit AudioContextInterface(QObject *parent = nullptr);
        QScopedPointer<AudioContextInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_H
