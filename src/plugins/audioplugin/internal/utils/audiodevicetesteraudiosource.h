#ifndef AUDIO_AUDIODEVICETESTERAUDIOSOURCE_H
#define AUDIO_AUDIODEVICETESTERAUDIOSOURCE_H

#include <TalcsCore/AudioSource.h>
#include <TalcsCore/AudioBuffer.h>

namespace Audio {

    class AudioDeviceTesterAudioSource : public talcs::AudioSource {
    public:
        explicit AudioDeviceTesterAudioSource();
        ~AudioDeviceTesterAudioSource() override;

        bool open(qint64 bufferSize, double sampleRate) override;
        void close() override;

        void playTestSound();

    protected:
        qint64 processReading(const talcs::AudioSourceReadData &readData) override;

    private:
        talcs::AudioBuffer m_sound;
        QAtomicInteger<qint64> m_pos = 0;
    };

}

#endif // AUDIO_AUDIODEVICETESTERAUDIOSOURCE_H
