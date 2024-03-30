#include "audiodevicetesteraudiosource.h"

namespace Audio {

    AudioDeviceTesterAudioSource::AudioDeviceTesterAudioSource() = default;

    AudioDeviceTesterAudioSource::~AudioDeviceTesterAudioSource() = default;

    bool AudioDeviceTesterAudioSource::open(qint64 bufferSize, double sampleRate) {
        static const double PI = std::acos(-1);
        m_sound.resize(1, qint64(sampleRate));
        double fadeIn = 0.005;
        double fadeOut = 0.005;
        double rate = std::pow(0.99, 20000.0 / sampleRate);
        qint64 i, j;
        for (i = 0; i < m_sound.sampleCount() && fadeIn < 1.0; i++) {
            m_sound.sampleAt(0, i) = float(std::sin(2.0 * PI * 440.0 / sampleRate * double(i)) * fadeIn);
            fadeIn /= rate;
        }
        for (j = m_sound.sampleCount() - 1; j >= 0 && fadeOut < 1.0; j--) {
            m_sound.sampleAt(0, j) = float(std::sin(2.0 * PI * 440.0 / sampleRate * double(j)) * fadeOut);
            fadeOut /= rate;
        }
        for (;i <= j; i++) {
            m_sound.sampleAt(0, i) = float(std::sin(2.0 * PI * 440.0 / sampleRate * double(i)));
        }
        return AudioStreamBase::open(bufferSize, sampleRate);
    }
    void AudioDeviceTesterAudioSource::close() {
        AudioStreamBase::close();
    }
    qint64 AudioDeviceTesterAudioSource::read(const talcs::AudioSourceReadData &readData) {
        qint64 pos = m_pos;
        qint64 length = qMin(readData.length, m_sound.sampleCount() - pos);
        for (int ch = 0; ch < readData.buffer->channelCount(); ch++) {
            readData.buffer->setSampleRange(ch, readData.startPos, length, m_sound, 0, pos);
            readData.buffer->clear(ch, readData.startPos + length, readData.length - length);
        }
        if (m_pos == pos)
            m_pos += length;
        return readData.length;
    }

    void AudioDeviceTesterAudioSource::playTestSound() {
        m_pos = 0;
    }
} // Audio