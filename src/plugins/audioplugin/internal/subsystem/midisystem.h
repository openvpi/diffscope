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
    class MidiBlockerFilter;

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
        void setReleaseMsec(int msec);
        int releaseMsec() const;

        void handleAudioMakeReady();

        void updateControl();

    signals:
        void makeReadyRequired();

    private:
        std::unique_ptr<talcs::MidiInputDevice> m_device;
        std::unique_ptr<talcs::MidiMessageIntegrator> m_integrator;
        talcs::MidiNoteSynthesizer *m_synthesizer;
        std::unique_ptr<talcs::MixerAudioSource> m_synthesizerMixer;

        std::unique_ptr<MidiControlFilter> m_midiControlFilter;
        std::unique_ptr<MidiBlockerFilter> m_midiBlockerFilter;

        void postSetDevice();

        void updateRateOnSampleRateChange(double sampleRate);

        Q_INVOKABLE void handleMakeReadyRequired();
    };

}

#endif // AUDIO_MIDISYSTEM_H
