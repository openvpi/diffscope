#include "vstconnectionsystem.h"

#include <QApplication>
#include <QJsonDocument>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioSourcePlayback.h>
#include <TalcsRemote/RemoteSocket.h>
#include <TalcsRemote/RemoteAudioDevice.h>
#include <TalcsRemote/RemoteEditor.h>

#include <QMWidgets/qmappextension.h>

#include <CoreApi/iloader.h>

namespace Audio {
    VSTConnectionSystem::VSTConnectionSystem(QObject *parent) : AbstractOutputSystem(parent) {
    }
    VSTConnectionSystem::~VSTConnectionSystem() {
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
        return true;
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
    bool VSTConnectionSystem::makeReady() {
        if (!m_dev) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot make ready because device is null";
            return false;
        }
        if (!m_dev->isOpen()) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because device is not opened by remote host";
            return false;
        }
        if (!m_preMixer->isOpen() && !m_preMixer->open(m_dev->bufferSize(), m_dev->sampleRate())) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because cannot open pre-mixer";
            return false;
        }
        if (!m_dev->isStarted() && !m_dev->start(m_playback.get())) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because cannot start audio device";
            return false;
        }
        return true;
    }
    void VSTConnectionSystem::setVSTAddOn(VSTAddOn *addOn) {
        m_vstAddOn = addOn;
    }
    VSTAddOn *VSTConnectionSystem::vstAddOn() const {
        return m_vstAddOn;
    }
    QPair<QString, QString> VSTConnectionSystem::hostSpecs() const {
        return m_hostSpecs;
    }
    QByteArray VSTConnectionSystem::getEditorData(bool *ok) {
        return QByteArray();
    }
    bool VSTConnectionSystem::setEditorData(const QByteArray &data) {
        return false;
    }
    void VSTConnectionSystem::handleRemoteDeviceRemoteOpened(qint64 bufferSize, double sampleRate, int maxChannelCount) {
        auto oldBufferSize = m_dev->bufferSize();
        auto oldSampleRate = m_dev->sampleRate();
        m_dev->open(bufferSize, sampleRate);
        if (bufferSize != oldBufferSize) {
            emit bufferSizeChanged(bufferSize);
        }
        if (!qFuzzyCompare(sampleRate, oldSampleRate)) {
            emit sampleRateChanged(sampleRate);
        }
        m_preMixer->open(bufferSize, sampleRate);
        emit deviceChanged();
    }
    void VSTConnectionSystem::setHostSpecs(const QString &hostExecutable, const QString &pluginFormat) {
        m_hostSpecs = {hostExecutable, pluginFormat};
        emit hostSpecsChanged(hostExecutable, pluginFormat);
    }
} // Audio