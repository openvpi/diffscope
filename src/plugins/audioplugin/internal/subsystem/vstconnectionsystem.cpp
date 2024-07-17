#include "vstconnectionsystem.h"

#include <QApplication>
#include <QJsonDocument>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/NoteSynthesizer.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AudioSourcePlayback.h>
#include <TalcsRemote/RemoteSocket.h>
#include <TalcsRemote/RemoteAudioDevice.h>
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
        m_integrator = std::make_unique<talcs::RemoteMidiMessageIntegrator>();
        m_synthesizer = new talcs::MidiNoteSynthesizer;
        m_integrator->setStream(m_synthesizer, true);
        m_synthesizerMixer = std::make_unique<talcs::MixerAudioSource>();
        m_synthesizerMixer->addSource(m_integrator.get());

        m_preMixer->addSource(m_integrator.get());
    }
    VSTConnectionSystem::~VSTConnectionSystem() {
        if (m_dev)
            m_dev->removeProcessInfoCallback(m_integrator.get());
        m_preMixer->removeAllSources();
    }
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
        auto socket = std::make_unique<talcs::RemoteSocket>(editorPort, pluginPort, this);
        if (!socket->startServer(QThread::idealThreadCount())) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: socket fails to start server";
            return false;
        }
        m_dev = new talcs::RemoteAudioDevice(socket.get(), "Host Audio", socket.get());
        m_dev->addProcessInfoCallback(m_integrator.get());
        syncSynthesizerPreference();
        connect(m_dev, &talcs::RemoteAudioDevice::remoteOpened, this ,&VSTConnectionSystem::handleRemoteDeviceRemoteOpened);
        m_editor = new talcs::RemoteEditor(
            socket.get(),
            [this](bool *ok) { return getEditorData(ok); },
            [this](const QByteArray &data) { return setEditorData(data); },
            socket.get());
        socket->bind("vstConnectionSystem", "setHostSpecs", [=](const std::string &hostExecutable, const std::string &pluginFormat) {
            setHostSpecs(QString::fromStdString(hostExecutable), QString::fromStdString(pluginFormat));
        });
        if (!socket->startClient()) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: socket fails to start client";
            return false;
        }
        m_socket = socket.release();
        qDebug() << "Audio::VSTConnectionSystem: waiting for connection" << "editorPort =" << editorPort << "pluginPort =" << pluginPort;
        return AbstractOutputSystem::initialize();
    }
    void VSTConnectionSystem::setApplicationInitializing(bool a) {
        m_isApplicationInitializing = a;
    }
    bool VSTConnectionSystem::isApplicationInitializing() const {
        return m_isApplicationInitializing;
    }
    talcs::RemoteSocket *VSTConnectionSystem::socket() const {
        return m_socket;
    }
    talcs::AudioDevice *VSTConnectionSystem::device() const {
        return m_dev;
    }
    talcs::RemoteAudioDevice *VSTConnectionSystem::remoteAudioDevice() const {
        return m_dev;
    }
    talcs::RemoteEditor *VSTConnectionSystem::remoteEditor() const {
        return m_editor;
    }
    talcs::RemoteMidiMessageIntegrator *VSTConnectionSystem::integrator() const {
        return m_integrator.get();
    }
    talcs::MidiNoteSynthesizer *VSTConnectionSystem::synthesizer() const {
        return m_synthesizer;
    }
    void VSTConnectionSystem::syncSynthesizerPreference() {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        m_synthesizer->noteSynthesizer()->setGenerator(static_cast<talcs::NoteSynthesizer::Generator>(obj["midiSynthesizerGenerator"].toInt()));
        m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(obj["midiSynthesizerAttackMsec"].toInt(10), m_dev && m_dev->isOpen() ? m_dev->sampleRate() : 48000));
        m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(obj["midiSynthesizerReleaseMsec"].toInt(50), m_dev && m_dev->isOpen() ? m_dev->sampleRate() : 48000));
        m_synthesizerMixer->setGain(talcs::Decibels::decibelsToGain(obj["midiSynthesizerAmplitude"].toDouble(-3)));
        if (qFuzzyIsNull(obj["midiSynthesizerFrequencyOfA"].toDouble())) {
            // TODO
        } else {
            m_synthesizer->setFrequencyOfA(obj["midiSynthesizerFrequencyOfA"].toDouble());
        }
    }
    bool VSTConnectionSystem::makeReady() {
        if (!m_dev) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot make ready because device is null";
            return false;
        }
        if (!m_dev->isOpen()) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot make ready because device is not opened by remote host";
            return false;
        }
        if (!m_deviceControlMixer->isOpen() && !m_deviceControlMixer->open(m_dev->bufferSize(), m_dev->sampleRate())) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot make ready because cannot open pre-mixer";
            return false;
        }
        if (!m_dev->isStarted() && !m_dev->start(m_playback.get())) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot make ready because cannot start audio device";
            return false;
        }
        return true;
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
    void VSTConnectionSystem::handleRemoteDeviceRemoteOpened(qint64 bufferSize, double sampleRate, int maxChannelCount) {
        auto oldBufferSize = m_dev->bufferSize();
        auto oldSampleRate = m_dev->sampleRate();
        m_dev->open(bufferSize, sampleRate);
        if (bufferSize != oldBufferSize) {
            emit bufferSizeChanged(bufferSize);
        }
        if (!qFuzzyCompare(sampleRate, oldSampleRate)) {
            const auto &settings = *Core::ILoader::instance()->settings();
            auto obj = settings["Audio"].toObject();
            setFileBufferingReadAheadSize(static_cast<qint64>(obj["fileBufferingReadAheadSize"].toInt() * sampleRate / 1000.0));
            m_synthesizer->noteSynthesizer()->setAttackRate(msecToRate(obj["midiSynthesizerAttackMsec"].toInt(10), sampleRate));
            m_synthesizer->noteSynthesizer()->setReleaseRate(msecToRate(obj["midiSynthesizerReleaseMsec"].toInt(50), sampleRate));
            emit sampleRateChanged(sampleRate);
        }
        m_deviceControlMixer->open(bufferSize, sampleRate);
        m_dev->start(m_playback.get());
        emit deviceChanged();
    }
    void VSTConnectionSystem::setHostSpecs(const QString &hostExecutable, const QString &pluginFormat) {
        m_hostSpecs = {hostExecutable, pluginFormat};
        emit hostSpecsChanged(hostExecutable, pluginFormat);
    }
}