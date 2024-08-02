#include "midisystem.h"

#include <QTimer>

#include <TalcsCore/NoteSynthesizer.h>
#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsMidi/MidiInputDevice.h>
#include <TalcsMidi/MidiMessageIntegrator.h>
#include <TalcsMidi/MidiNoteSynthesizer.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/internal/audiohelpers.h>
#include <audioplugin/internal/audiosettings.h>
#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>

namespace Audio::Internal {

    class MidiControlFilter : public talcs::MidiMessageListener {
    protected:
        bool processDeviceWillStart(talcs::MidiInputDevice *device) override {
            return true;
        }
        void processDeviceStopped() override {
        }
        bool processMessage(const talcs::MidiMessage &message) override {
            return false;
        }
        void processError(const QString &errorString) override {
        }
    };

    static qint64 msecToSample(int msec, double sampleRate = {}) {
        auto audioDevice = AudioSystem::outputSystem()->outputContext()->device();
        sampleRate = qFuzzyIsNull(sampleRate) ? audioDevice && audioDevice->isOpen() ? audioDevice->sampleRate() : 48000.0 : sampleRate;
        return AudioHelpers::msecToSample(msec, sampleRate);
    }

    MidiSystem::MidiSystem(QObject *parent) : QObject(parent) {
        m_integrator = std::make_unique<talcs::MidiMessageIntegrator>();
        m_synthesizer = new talcs::MidiNoteSynthesizer;
        m_integrator->setStream(m_synthesizer, true);
        m_synthesizerMixer = std::make_unique<talcs::MixerAudioSource>();
        m_synthesizerMixer->addSource(m_integrator.get());

        m_midiControlFilter = std::make_unique<MidiControlFilter>();
        m_integrator->addFilter(m_midiControlFilter.get());
    }
    MidiSystem::~MidiSystem() {
        m_device.reset(); // due to delete order issue
    }
    bool MidiSystem::initialize() {
        AudioSystem::outputSystem()->outputContext()->preMixer()->addSource(m_synthesizerMixer.get());
        connect(AudioSystem::outputSystem()->outputContext(), &talcs::OutputContext::sampleRateChanged, this, &MidiSystem::updateRateOnSampleRateChange);
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(AudioSettings::midiSynthesizerGenerator()));

        m_synthesizer->noteSynthesizer()->setAttackTime(msecToSample(AudioSettings::midiSynthesizerAttackMsec()));
        m_synthesizer->noteSynthesizer()->setDecayTime(msecToSample(AudioSettings::midiSynthesizerDecayMsec()));
        m_synthesizer->noteSynthesizer()->setDecayRatio(AudioSettings::midiSynthesizerDecayRatio());
        m_synthesizer->noteSynthesizer()->setReleaseTime(msecToSample(AudioSettings::midiSynthesizerReleaseMsec()));
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(AudioSettings::midiSynthesizerAmplitude()));
        if (qFuzzyIsNull(AudioSettings::midiSynthesizerFrequencyOfA())) {
            // TODO
        } else {
            m_synthesizer->setFrequencyOfA(AudioSettings::midiSynthesizerFrequencyOfA());
        }

        auto savedDeviceIndex = AudioSettings::midiDeviceIndex();
        qDebug() << "Audio::MidiSystem: saved device index" << savedDeviceIndex;
        auto deviceCount = talcs::MidiInputDevice::devices().size();
        for(int i = -1; i < deviceCount; i++) {
            if (i == savedDeviceIndex)
                continue;
            int deviceIndex = i;
            if (deviceIndex == -1)
                deviceIndex = savedDeviceIndex;
            if (deviceIndex == -1)
                deviceIndex = 0;
            auto dev = std::make_unique<talcs::MidiInputDevice>(deviceIndex);
            if (dev->open()) {
                m_device = std::move(dev);
                break;
            }
        }
        if (!m_device) {
            qWarning() << "Audio::MidiSystem: fatal: no available device";
            return false;
        }
        qDebug() << "Audio::MidiSystem: MIDI device initialized" << m_device->deviceIndex() << m_device->name();
        postSetDevice();
        return true;
    }
    talcs::MidiInputDevice *MidiSystem::device() {
        return m_device.get();
    }
    bool MidiSystem::setDevice(int deviceIndex) {
        auto dev = std::make_unique<talcs::MidiInputDevice>(deviceIndex);
        if (!dev->open()) {
            return false;
        }
        qDebug() << "Audio::MidiSystem: MIDI device changed" << dev->name();
        m_device = std::move(dev);
        AudioSettings::setMidiDeviceIndex(deviceIndex);
        postSetDevice();
        return true;
    }
    void MidiSystem::postSetDevice() {
        m_device->listener()->addFilter(m_integrator.get());
    }
    talcs::MidiMessageIntegrator *MidiSystem::integrator() {
        return m_integrator.get();
    }
    talcs::MidiNoteSynthesizer *MidiSystem::synthesizer() {
        return m_synthesizer;
    }
    void MidiSystem::setGenerator(int g) {
        AudioSettings::setMidiSynthesizerGenerator(g);
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(g));
    }
    int MidiSystem::generator() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerGenerator();
    }
    void MidiSystem::setAmplitudeDecibel(double dB) {
        AudioSettings::setMidiSynthesizerAmplitude(dB);
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(static_cast<float>(dB)));
    }
    double MidiSystem::amplitudeDecibel() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerAmplitude();
    }
    void MidiSystem::setAttackMsec(int msec) {
        AudioSettings::setMidiSynthesizerAttackMsec(msec);
        m_synthesizer->noteSynthesizer()->setAttackTime(msecToSample(msec));
    }
    int MidiSystem::attackMsec() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerAttackMsec();
    }
    void MidiSystem::setDecayMsec(int msec) {
        AudioSettings::setMidiSynthesizerDecayMsec(msec);
        m_synthesizer->noteSynthesizer()->setDecayTime(msecToSample(msec));
    }
    int MidiSystem::decayMsec() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerDecayMsec();
    }
    void MidiSystem::setDecayRatio(double ratio) {
        AudioSettings::setMidiSynthesizerDecayRatio(ratio);
        m_synthesizer->noteSynthesizer()->setDecayRatio(ratio);
    }
    double MidiSystem::decayRatio() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerDecayRatio();
    }
    void MidiSystem::setReleaseMsec(int msec) {
        AudioSettings::setMidiSynthesizerReleaseMsec(msec);
        m_synthesizer->noteSynthesizer()->setReleaseTime(msecToSample(msec));
    }
    int MidiSystem::releaseMsec() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerReleaseMsec();
    }
    void MidiSystem::setFrequencyOfA(double frequency) {
        AudioSettings::setMidiSynthesizerFrequencyOfA(frequency);
        if (qFuzzyIsNull(frequency)) {
            // TODO adjust by cent shift
        } else {
            m_synthesizer->setFrequencyOfA(frequency);
        }

    }
    double MidiSystem::frequencyOfA() const {
        Q_UNUSED(this)
        return AudioSettings::midiSynthesizerFrequencyOfA();
    }
    void MidiSystem::updateControl() {
    }
    void MidiSystem::updateRateOnSampleRateChange(double sampleRate) {
        m_synthesizer->noteSynthesizer()->setAttackTime(msecToSample(AudioSettings::midiSynthesizerAttackMsec(), sampleRate));
        m_synthesizer->noteSynthesizer()->setDecayTime(msecToSample(AudioSettings::midiSynthesizerDecayMsec(), sampleRate));
        m_synthesizer->noteSynthesizer()->setReleaseTime(msecToSample(AudioSettings::midiSynthesizerReleaseMsec(), sampleRate));
    }
}