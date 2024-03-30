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
        qint64 read(const talcs::AudioSourceReadData &readData) override;

        void playTestSound();

    private:
        talcs::AudioBuffer m_sound;
        QAtomicInteger<qint64> m_pos = 0;
    };

} // Audio

#endif // AUDIO_AUDIODEVICETESTERAUDIOSOURCE_H
