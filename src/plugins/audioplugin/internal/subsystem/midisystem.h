#ifndef AUDIO_MIDISYSTEM_H
#define AUDIO_MIDISYSTEM_H

#include <memory>

#include <QObject>

namespace talcs {

    class MixerAudioSource;

    class MidiInputDevice;
    class MidiMessageIntegrator;
    class MidiNoteSynthesizer;
}

namespace Audio::Internal {

    class MidiControlFilter;

    class MidiSystem : public QObject {
        Q_OBJECT
    public:
        explicit MidiSystem(QObject *parent = nullptr);
        ~MidiSystem() override;

        bool initialize();
        talcs::MidiInputDevice *device();
        bool setDevice(int deviceIndex);
        talcs::MidiMessageIntegrator *integrator();
        talcs::MidiNoteSynthesizer *synthesizer();

        void setGenerator(int g);
        int generator() const;
        void setAmplitudeDecibel(double dB);
        double amplitudeDecibel() const;
        void setAttackMsec(int msec);
        int attackMsec() const;
        void setDecayMsec(int msec);
        int decayMsec() const;
        void setDecayRatio(double ratio);
        double decayRatio() const;
        void setReleaseMsec(int msec);
        int releaseMsec() const;
        void setFrequencyOfA(double frequency);
        double frequencyOfA() const;

        void updateControl();

    private:
        std::unique_ptr<talcs::MidiInputDevice> m_device;
        std::unique_ptr<talcs::MidiMessageIntegrator> m_integrator;
        talcs::MidiNoteSynthesizer *m_synthesizer;
        std::unique_ptr<talcs::MixerAudioSource> m_synthesizerMixer;

        std::unique_ptr<MidiControlFilter> m_midiControlFilter;

        void postSetDevice();

        void updateRateOnSampleRateChange(double sampleRate);
    };

}

#endif // AUDIO_MIDISYSTEM_H
