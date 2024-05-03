#ifndef AUDIO_TRACKINTERFACE_H
#define AUDIO_TRACKINTERFACE_H

#include <CoreApi/iexecutive.h>

#include <audioplugin/audioglobal.h>

namespace QDspx {
    class TrackEntity;
    class AudioClipEntity;
}

namespace talcs {
    class PositionableMixerAudioSource;
    class AudioSourceClipSeries;
}

namespace Audio {

    class ITrackAddOn;

    class AudioContextInterface;

    class AudioClipInterface;

    class TrackInterfacePrivate;

    class AUDIO_EXPORT TrackInterface : public Core::IExecutive {
        Q_OBJECT
        Q_DECLARE_PRIVATE(TrackInterface)
        friend class Core::IExecutiveRegistry<TrackInterface>;
        using AddOnType = ITrackAddOn;
    public:
        ~TrackInterface() override;

        QDspx::TrackEntity *entity() const;
        AudioContextInterface *context() const;

        talcs::PositionableMixerAudioSource *trackMixer() const;
        talcs::AudioSourceClipSeries *clipSeries() const;

        QList<AudioClipInterface *> clips() const;
        AudioClipInterface *getClip(QDspx::AudioClipEntity *entity) const;

    protected:
        explicit TrackInterface(QDspx::TrackEntity *entity, AudioContextInterface *context, QObject *parent = nullptr);
        explicit TrackInterface(TrackInterfacePrivate &d, QObject *parent);

    private:
        QScopedPointer<TrackInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_TRACKINTERFACE_H
