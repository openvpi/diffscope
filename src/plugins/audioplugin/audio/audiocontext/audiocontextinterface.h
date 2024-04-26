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
    class IWindow;
    template <class T> class IExecutiveRegistry;
}

namespace QDspx {
    class TrackEntity;
}

namespace Audio {

    class ProjectAddOn;

    class TrackInterface;

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

        Core::IWindow *windowHandle() const;
        static AudioContextInterface *get(Core::IWindow *win);

        Core::IExecutiveRegistry<TrackInterface> *trackRegistry() const;

        QList<TrackInterface *> tracks() const;
        TrackInterface *getTrack(QDspx::TrackEntity *entity) const;

    private:
        explicit AudioContextInterface(QObject *parent = nullptr);
        QScopedPointer<AudioContextInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_AUDIOCONTEXTINTERFACE_H
