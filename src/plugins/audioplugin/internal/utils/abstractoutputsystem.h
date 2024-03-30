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

    class AudioDeviceTesterAudioSource;

    class AbstractOutputSystem : public QObject {
        Q_OBJECT
    public:
        explicit AbstractOutputSystem(QObject *parent = nullptr);
        ~AbstractOutputSystem() override;

        virtual bool initialize() = 0;
        virtual talcs::AudioDevice *device() const = 0;
        talcs::MixerAudioSource *preMixer() const;

        virtual bool makeReady() = 0;

        void testDevice();

    Q_SIGNALS:
        void bufferSizeChanged(qint64 bufferSize);
        void sampleRateChanged(double sampleRate);
        void deviceChanged();

    protected:
        talcs::MixerAudioSource *m_preMixer;
        std::unique_ptr<talcs::AudioSourcePlayback> m_playback;
        AudioDeviceTesterAudioSource *m_deviceTester;
    };

} // Audio

#endif // AUDIO_ABSTRACTOUTPUTSYSTEM_H
