#ifndef AUDIO_PROJECTADDON_H
#define AUDIO_PROJECTADDON_H

#include <memory>

#include <TalcsCore/MetronomeAudioSource.h>
#include <TalcsCore/BufferingAudioSource.h>

#include <QMCore/qmchronomap.h>

#include <CoreApi/iwindow.h>

namespace talcs {
    class MixerAudioSource;
    class TransportAudioSource;
    class PositionableMixerAudioSource;
    class PositionableAudioSource;
    class AbstractAudioFormatIO;
}

namespace Audio {
    class AudioContextInterface;

    class IAudioContextAddOn;
}

namespace Audio::Internal {

    class ProjectAddOn : public Core::IWindowAddOn {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit ProjectAddOn(QObject *parent = nullptr);
        ~ProjectAddOn();

        void initialize() override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;

        bool isVST() const;
        talcs::MixerAudioSource *preMixer() const;
        talcs::TransportAudioSource *transportAudioSource() const;
        talcs::PositionableMixerAudioSource *postMixer() const;
        talcs::PositionableMixerAudioSource *masterTrackMixer() const;

        talcs::PositionableAudioSource *postGetFormat(talcs::AbstractAudioFormatIO *io, const QString &filename, bool isInternal);

    private:
        std::unique_ptr<talcs::MixerAudioSource> m_preMixer;
        talcs::TransportAudioSource *m_tpSrc;
        talcs::PositionableMixerAudioSource *m_postMixer;
        talcs::PositionableMixerAudioSource *m_masterTrackControlMixer;
        talcs::PositionableMixerAudioSource *m_masterTrackMixer;

        AudioContextInterface *m_audioContextInterface;

        struct FileInfo {
            QString filename;
            talcs::BufferingAudioSource *bufSrc;
            talcs::AbstractAudioFormatIO *io;
            bool isInternal;
        };

        QMChronoMap<talcs::BufferingAudioSource *, FileInfo> files;

        void handleEntityGainChange(double gainDecibel) const;
        void handleEntityPanChange(double pan) const;
        void handleEntityMuteChange(bool isMute) const;
    };

}

#endif // AUDIO_PROJECTADDON_H
