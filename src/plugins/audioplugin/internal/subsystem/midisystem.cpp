#include "midisystem.h"

#include <QTimer>

#include <TalcsCore/NoteSynthesizer.h>
#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsMidi/MidiInputDevice.h>
#include <TalcsMidi/MidiMessageIntegrator.h>
#include <TalcsMidi/MidiNoteSynthesizer.h>

#include <icore.h>
#include <CoreApi/iloader.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/outputsysteminterface.h>

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

    static double msecToRate(int msec, double sampleRate) {
        if (msec == 0)
            return 0.005;
        return std::pow(0.005, 1000.0 / (msec * sampleRate));
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
        IAudio::instance()->outputSystemInterface(false)->preMixer()->addSource(m_synthesizerMixer.get());
        connect(IAudio::instance()->outputSystemInterface(false), &OutputSystemInterface::sampleRateChanged, this, &MidiSystem::updateRateOnSampleRateChange);
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(obj["midiSynthesizerGenerator"].toInt()));
        auto audioDevice = IAudio::instance()->outputSystemInterface(false)->audioDevice();
        m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(obj["midiSynthesizerAttackMsec"].toInt(10), audioDevice && audioDevice->isOpen() ? audioDevice->sampleRate() : 48000));
        m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(obj["midiSynthesizerReleaseMsec"].toInt(50), audioDevice && audioDevice->isOpen() ? audioDevice->sampleRate() : 48000));
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(static_cast<float>(obj["midiSynthesizerAmplitude"].toDouble())));

        auto deviceIndex = obj["midiDeviceIndex"].toInt(-1);
        qDebug() << "Audio::MidiSystem: saved device index" << deviceIndex;
        if (deviceIndex == -1)
            deviceIndex = 0;
        auto deviceCount = talcs::MidiInputDevice::devices().size();
        for(; deviceIndex < deviceCount; deviceIndex++) {
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
        qDebug() << "Audio::MidiSystem: MIDI device initialized" << m_device->name();
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
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["midiDeviceIndex"] = deviceIndex;
        settings["Audio"] = obj;
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
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["midiSynthesizerGenerator"] = g;
        settings["Audio"] = obj;
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(g));
    }
    int MidiSystem::generator() const {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        return obj["midiSynthesizerGenerator"].toInt();
    }
    void MidiSystem::setAmplitudeDecibel(double dB) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["midiSynthesizerAmplitude"] = dB;
        settings["Audio"] = obj;
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(static_cast<float>(dB)));
    }
    double MidiSystem::amplitudeDecibel() const {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        return obj["midiSynthesizerAmplitude"].toDouble();
    }
    void MidiSystem::setAttackMsec(int msec) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["midiSynthesizerAttackMsec"] = msec;
        settings["Audio"] = obj;
        auto audioDevice = IAudio::instance()->outputSystemInterface(false)->audioDevice();
        m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(msec, audioDevice && audioDevice->isOpen() ? audioDevice->sampleRate() : 48000));
    }
    int MidiSystem::attackMsec() const {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        return obj["midiSynthesizerAttackMsec"].toInt(10);
    }
    void MidiSystem::setReleaseMsec(int msec) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["midiSynthesizerReleaseMsec"] = msec;
        settings["Audio"] = obj;
        auto audioDevice = IAudio::instance()->outputSystemInterface(false)->audioDevice();
        m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(msec, audioDevice && audioDevice->isOpen() ? audioDevice->sampleRate() : 48000));
    }
    int MidiSystem::releaseMsec() const {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        return obj["midiSynthesizerReleaseMsec"].toInt(50);
    }
    void MidiSystem::updateControl() {
    }
    void MidiSystem::updateRateOnSampleRateChange(double sampleRate) {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(obj["midiSynthesizerAttackMsec"].toInt(10), sampleRate));
        m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(obj["midiSynthesizerReleaseMsec"].toInt(10), sampleRate));
    }
}