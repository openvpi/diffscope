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
    VSTConnectionSystem::VSTConnectionSystem(QObject *parent) : QObject(parent) {
    }
    VSTConnectionSystem::~VSTConnectionSystem() {
        m_preMixer = new talcs::MixerAudioSource(this);
        m_playback = std::make_unique<talcs::AudioSourcePlayback>(m_preMixer, false, false);
    }
    bool VSTConnectionSystem::initialize() {
        auto vstConfigPath = qAppExt->appDataDir() + "/vstconfig.json";
        qDebug() << "Audio::VSTConnectionSystem: VST config path" << vstConfigPath;
        QFile vstConfigFile(vstConfigPath);
        if (!vstConfigFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Audio::VSTConnectionSystem: fatal: cannot open VST config file";
            return false;
        }
        auto settingObj = std::make_unique<QJsonObject>(Core::ILoader::instance()->settings()->value("Audio").toObject());
        if (!settingObj->contains("vstEditorPort"))
            settingObj->insert("vstEditorPort", 28081);
        if (!settingObj->contains("vstPluginPort"))
            settingObj->insert("vstPluginPort", 28082);
        auto editorPort = settingObj->value("vstEditorPort").toInt();
        auto pluginPort = settingObj->value("vstPluginPort").toInt();
        QJsonDocument doc({
            {"editor",      QApplication::applicationFilePath()},
            {"editorPort",  editorPort                 },
            {"pluginPort",  pluginPort                 },
            {"threadCount", QThread::idealThreadCount()},
        });
        vstConfigFile.write(doc.toJson());
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
    talcs::RemoteAudioDevice *VSTConnectionSystem::device() const {
        return m_dev;
    }
    talcs::MixerAudioSource *VSTConnectionSystem::preMixer() const {
        return m_preMixer;
    }
    void VSTConnectionSystem::testDevice() {
        // TODO
    }
    void VSTConnectionSystem::setVSTAddOn(VSTAddOn *addOn) {
        m_vstAddOn = addOn;
    }
    VSTAddOn *VSTConnectionSystem::vstAddOn() const {
        return m_vstAddOn;
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
            emit remoteBufferSizeChanged(bufferSize);
        }
        if (!qFuzzyCompare(sampleRate, oldSampleRate)) {
            emit remoteSampleRateChanged(sampleRate);
        }
        m_preMixer->open(bufferSize, sampleRate);
        emit deviceRemoteOpenedHandled();
    }
} // Audio