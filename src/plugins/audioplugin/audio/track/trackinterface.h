#ifndef AUDIO_TRACKINTERFACE_H
#define AUDIO_TRACKINTERFACE_H

#include <CoreApi/iexecutive.h>

#include <audioplugin/audioglobal.h>

class QMutex;

namespace QDspx {
    class TrackEntity;
    class AudioClipEntity;
}

namespace talcs {
    class PositionableMixerAudioSource;
    class AudioSourceClipSeries;
    class DspxTrackContext;
}

namespace Audio {

    class AudioContextInterface;
    class AudioClipInterface;

    class ITrackAddOn;
    class TrackInterfacePrivate;

    class AUDIO_EXPORT TrackInterface : public Core::IExecutive {
        Q_OBJECT
        Q_DECLARE_PRIVATE(TrackInterface)
        friend class Core::IExecutiveRegistry<TrackInterface>;
        friend class AudioContextInterface;
        using AddOnType = ITrackAddOn;
    public:
        ~TrackInterface() override;

        QDspx::TrackEntity *entity() const;

        AudioContextInterface *audioContextInterface() const;

        talcs::PositionableMixerAudioSource *trackMixer() const;
        talcs::AudioSourceClipSeries *clipSeries() const;

        QList<AudioClipInterface *> clips() const;
        AudioClipInterface *getAudioClipInterfaceFromEntity(QDspx::AudioClipEntity *entity) const;

    protected:
        explicit TrackInterface(QDspx::TrackEntity *entity, AudioContextInterface *audioContextInterface, talcs::DspxTrackContext *trackContext);
        explicit TrackInterface(TrackInterfacePrivate &d, QObject *parent);

    private:
        QScopedPointer<TrackInterfacePrivate> d_ptr;
    };

}

#endif // AUDIO_TRACKINTERFACE_H
