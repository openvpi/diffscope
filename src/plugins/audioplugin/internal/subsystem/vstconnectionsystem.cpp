#include "vstconnectionsystem.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/NoteSynthesizer.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AudioSourcePlayback.h>
#include <TalcsRemote/RemoteSocket.h>
#include <TalcsRemote/RemoteAudioDevice.h>
#include <TalcsRemote/RemoteOutputContext.h>
#include <TalcsRemote/RemoteEditor.h>
#include <TalcsRemote/RemoteMidiMessageIntegrator.h>
#include <TalcsMidi/MidiNoteSynthesizer.h>

#include <QMWidgets/qmappextension.h>

#include <audioplugin/internal/audiohelpers.h>
#include <audioplugin/internal/audiosettings.h>

namespace Audio::Internal {

    VSTConnectionSystem::VSTConnectionSystem(QObject *parent) : AbstractOutputSystem(parent) {
        m_remoteOutputContext = std::make_unique<talcs::RemoteOutputContext>();
        setContext(m_remoteOutputContext.get());

        m_integrator = std::make_unique<talcs::RemoteMidiMessageIntegrator>();
        m_synthesizer = std::make_unique<talcs::MidiNoteSynthesizer>();
        m_integrator->setStream(m_synthesizer.get());
        m_synthesizerMixer = std::make_unique<talcs::MixerAudioSource>();
        m_synthesizerMixer->addSource(m_integrator.get());

        context()->preMixer()->addSource(m_integrator.get());
    }
    VSTConnectionSystem::~VSTConnectionSystem() = default;

    bool VSTConnectionSystem::createVSTConfig() {
        auto vstConfigPath = qAppExt->appDataDir() + "/vstconfig.json";
        qDebug() << "Audio::VSTConnectionSystem: VST config path" << vstConfigPath;
        QFile vstConfigFile(vstConfigPath);
        if (!vstConfigFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot open VST config file";
            return false;
        }
        QJsonDocument doc({
            {"editor",      QApplication::applicationFilePath()},
            {"editorPort",  AudioSettings::vstEditorPort()},
            {"pluginPort",  AudioSettings::vstPluginPort()},
            {"threadCount", QThread::idealThreadCount()},
            {"theme",       AudioSettings::vstPluginEditorUsesCustomTheme() ? AudioSettings::vstTheme() : QJsonValue::Null},
        });
        vstConfigFile.write(doc.toJson());
        return true;
    }
    bool VSTConnectionSystem::initialize() {
        if (!createVSTConfig())
            return false;
        auto editorPort = AudioSettings::vstEditorPort();
        auto pluginPort = AudioSettings::vstPluginPort();

        setFileBufferingReadAheadSize(AudioSettings::fileBufferingReadAheadSize());
        if (!m_remoteOutputContext->initialize(editorPort, pluginPort)) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: socket fails to start server";
            return false;
        }
        syncSynthesizerPreference();
        m_editor = std::make_unique<talcs::RemoteEditor>(
            m_remoteOutputContext->socket(),
            [this](bool *ok) { return getEditorData(ok); },
            [this](const QByteArray &data) { return setEditorData(data); });
        m_remoteOutputContext->socket()->bind("vstConnectionSystem", "setHostSpecs", [=](const std::string &hostExecutable, const std::string &pluginFormat) {
            setHostSpecs(QString::fromStdString(hostExecutable), QString::fromStdString(pluginFormat));
        });
        if (!m_remoteOutputContext->establishConnection()) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: socket fails to start client";
            return false;
        }
        qDebug() << "Audio::VSTConnectionSystem: waiting for connection" << "editorPort =" << editorPort << "pluginPort =" << pluginPort;
        return true;
    }
    void VSTConnectionSystem::setApplicationInitializing(bool a) {
        m_isApplicationInitializing = a;
    }
    bool VSTConnectionSystem::isApplicationInitializing() const {
        return m_isApplicationInitializing;
    }
    talcs::RemoteEditor *VSTConnectionSystem::remoteEditor() const {
        return m_editor.get();
    }
    talcs::RemoteMidiMessageIntegrator *VSTConnectionSystem::integrator() const {
        return m_integrator.get();
    }
    talcs::MidiNoteSynthesizer *VSTConnectionSystem::synthesizer() const {
        return m_synthesizer.get();
    }

    void VSTConnectionSystem::syncSynthesizerPreference() {
        auto dev = m_remoteOutputContext->device();
        auto sampleRate = dev && dev->isOpen() ? dev->sampleRate() : 48000;
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(AudioSettings::midiSynthesizerGenerator()));
        m_synthesizer->noteSynthesizer()->setAttackTime(AudioHelpers::msecToSample(AudioSettings::midiSynthesizerAttackMsec(), sampleRate));
        m_synthesizer->noteSynthesizer()->setDecayTime(AudioHelpers::msecToSample(AudioSettings::midiSynthesizerDecayMsec(), sampleRate));
        m_synthesizer->noteSynthesizer()->setDecayRatio(AudioSettings::midiSynthesizerDecayRatio());
        m_synthesizer->noteSynthesizer()->setReleaseTime(AudioHelpers::msecToSample(AudioSettings::midiSynthesizerReleaseMsec(), sampleRate));
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(AudioSettings::midiSynthesizerAmplitude()));
        if (qFuzzyIsNull(AudioSettings::midiSynthesizerFrequencyOfA())) {
            // TODO
        } else {
            m_synthesizer->setFrequencyOfA(AudioSettings::midiSynthesizerFrequencyOfA());
        }
    }
    void VSTConnectionSystem::setVSTAddOn(ProjectAddOn *addOn) {
        m_vstAddOn = addOn;
    }
    ProjectAddOn *VSTConnectionSystem::vstAddOn() const {
        return m_vstAddOn;
    }
    QPair<QString, QString> VSTConnectionSystem::hostSpecs() const {
        return m_hostSpecs;
    }
    QByteArray VSTConnectionSystem::getEditorData(bool *ok) {
        return QByteArray(); // TODO
    }
    bool VSTConnectionSystem::setEditorData(const QByteArray &data) {
        return false; // TODO
    }
    void VSTConnectionSystem::handleSampleRateChange(double sampleRate) {
        m_synthesizer->noteSynthesizer()->setAttackTime(AudioHelpers::msecToSample(AudioSettings::midiSynthesizerAttackMsec(), sampleRate));
        m_synthesizer->noteSynthesizer()->setDecayTime(AudioHelpers::msecToSample(AudioSettings::midiSynthesizerDecayMsec(), sampleRate));
        m_synthesizer->noteSynthesizer()->setReleaseTime(AudioHelpers::msecToSample(AudioSettings::midiSynthesizerReleaseMsec(), sampleRate));
    }
    void VSTConnectionSystem::setHostSpecs(const QString &hostExecutable, const QString &pluginFormat) {
        m_hostSpecs = {hostExecutable, pluginFormat};
        emit hostSpecsChanged(hostExecutable, pluginFormat);
    }
}