#include "vstconnectionsystem.h"

#include <QApplication>
#include <QJsonDocument>

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

#include <CoreApi/iloader.h>

namespace Audio::Internal {

    static double msecToRate(int msec, double sampleRate) {
        if (msec == 0)
            return 0.005;
        return std::pow(0.005, 1000.0 / (msec * sampleRate));
    }

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
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        if (!obj.contains("vstEditorPort"))
            obj.insert("vstEditorPort", 28081);
        if (!obj.contains("vstPluginPort"))
            obj.insert("vstPluginPort", 28082);
        settings["Audio"] = obj;
        QJsonDocument doc({
            {"editor",      QApplication::applicationFilePath()},
            {"editorPort",  obj.value("vstEditorPort")},
            {"pluginPort",  obj.value("vstPluginPort")},
            {"threadCount", QThread::idealThreadCount()},
            {"theme",       obj.value("vstPluginEditorUsesCustomTheme").toBool() ? obj.value("vstTheme") : QJsonValue::Null},
        });
        vstConfigFile.write(doc.toJson());
        return true;
    }
    bool VSTConnectionSystem::initialize() {
        if (!createVSTConfig())
            return false;
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        auto editorPort = obj["vstEditorPort"].toInt();
        auto pluginPort = obj["vstPluginPort"].toInt();

        setFileBufferingReadAheadSize(obj["fileBufferingReadAheadSize"].toInt());
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
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        auto dev = m_remoteOutputContext->device();
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(obj["midiSynthesizerGenerator"].toInt()));
        m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(obj["midiSynthesizerAttackMsec"].toInt(10), dev && dev->isOpen() ? dev->sampleRate() : 48000));
        m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(obj["midiSynthesizerReleaseMsec"].toInt(50), dev && dev->isOpen() ? dev->sampleRate() : 48000));
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(obj["midiSynthesizerAmplitude"].toDouble(-3)));
        if (qFuzzyIsNull(obj["midiSynthesizerFrequencyOfA"].toDouble())) {
            // TODO
        } else {
            m_synthesizer->setFrequencyOfA(obj["midiSynthesizerFrequencyOfA"].toDouble());
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
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(obj["midiSynthesizerAttackMsec"].toInt(10), sampleRate));
        m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(obj["midiSynthesizerReleaseMsec"].toInt(50), sampleRate));
    }
    void VSTConnectionSystem::setHostSpecs(const QString &hostExecutable, const QString &pluginFormat) {
        m_hostSpecs = {hostExecutable, pluginFormat};
        emit hostSpecsChanged(hostExecutable, pluginFormat);
    }
}