#ifndef AUDIO_DEVICETESTER_H
#define AUDIO_DEVICETESTER_H

#include <limits>

#include <QObject>

#include <TalcsCore/AudioSource.h>
#include <TalcsCore/AudioBuffer.h>

namespace Audio {

    class OutputSystemInterface;

    class DeviceTester : public QObject, public talcs::AudioSource {
        Q_OBJECT
    public:
        explicit DeviceTester(OutputSystemInterface *outputSystemInterface, QObject *parent = nullptr);
        ~DeviceTester() override;

        bool open(qint64 bufferSize, double sampleRate) override;
        void close() override;

        void playTestSound();
        static void playTestSound(bool isVST);

    protected:
        qint64 processReading(const talcs::AudioSourceReadData &readData) override;

    private:
        OutputSystemInterface *m_outputSystemInterface;
        talcs::AudioBuffer m_sound;
        QAtomicInteger<qint64> m_pos = -1;
    };

}

#endif // AUDIO_DEVICETESTER_H
