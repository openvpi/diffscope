#ifndef AUDIO_ABSTRACTOUTPUTSYSTEM_H
#define AUDIO_ABSTRACTOUTPUTSYSTEM_H

#include <memory>

#include <QObject>

namespace talcs {
    class MixerAudioSource;
    class AudioSourcePlayback;
    class AudioDevice;
}

namespace Audio {

    class AbstractOutputSystem : public QObject {
        Q_OBJECT
    public:
        explicit AbstractOutputSystem(QObject *parent = nullptr);
        ~AbstractOutputSystem() override;

        virtual bool initialize();
        virtual talcs::AudioDevice *device() const = 0;
        talcs::MixerAudioSource *preMixer() const;

        virtual bool makeReady() = 0;

        void setGainAndPan(float gain, float pan);
        float gain() const;
        float pan() const;

    Q_SIGNALS:
        void bufferSizeChanged(qint64 bufferSize);
        void sampleRateChanged(double sampleRate);
        void deviceChanged();

    protected:
        talcs::MixerAudioSource *m_deviceControlMixer;
        talcs::MixerAudioSource *m_preMixer;
        std::unique_ptr<talcs::AudioSourcePlayback> m_playback;
    };

}

#endif // AUDIO_ABSTRACTOUTPUTSYSTEM_H
