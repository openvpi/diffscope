#include "outputsystem.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDevice/AudioDriver.h>
#include <TalcsDevice/AudioDriverManager.h>
#include <TalcsDevice/AudioSourcePlayback.h>

#include <icore.h>
#include <CoreApi/iloader.h>

#include "sdlaudiodevicedisplaynamehelper.h"

namespace Audio {
    OutputSystem::OutputSystem(QObject *parent) : AbstractOutputSystem(parent) {
        m_drvMgr = talcs::AudioDriverManager::createBuiltInDriverManager(this);

    }
    OutputSystem::~OutputSystem() {
        if (m_dev)
            m_dev->close();
        talcs::AudioDeviceLocker locker(m_dev.get());
    }
    bool OutputSystem::initialize() {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        m_adoptedBufferSize = obj["adoptedBufferSize"].toInt();
        m_adoptedSampleRate = obj["adoptedSampleRate"].toDouble();
        auto savedDriverName = obj["driverName"].toString();
        auto savedDeviceName = obj["deviceName"].toString();
        if (!obj.contains("deviceGain"))
            obj["deviceGain"] = 1.0f;
        if (!obj.contains("devicePan"))
            obj["devicePan"] = 0.0f;
        settings["Audio"] = obj;
        setGain(static_cast<float>(obj["deviceGain"].toDouble()));
        setPan(static_cast<float>(obj["devicePan"].toDouble()));
        setFileBufferingMsec(obj["fileBufferingMsec"].toDouble());
        do {
            if (savedDriverName.isEmpty() || savedDeviceName.isEmpty())
                break;
            m_drv = m_drvMgr->driver(savedDriverName);
            if (!m_drv || !m_drv->initialize()) {
                m_drv = nullptr;
                break;
            }
            std::unique_ptr<talcs::AudioDevice> savedDevice(m_drv->createDevice(savedDeviceName));
            if (!savedDevice || !savedDevice->isInitialized())
                break;
            auto savedBufferSize = m_adoptedBufferSize == 0 ? savedDevice->preferredBufferSize() : m_adoptedBufferSize;
            auto savedSampleRate = qFuzzyIsNull(m_adoptedSampleRate) ? savedDevice->preferredSampleRate() : m_adoptedSampleRate;
            if (!savedDevice->open(savedBufferSize, savedSampleRate) && !savedDevice->open(savedDevice->preferredBufferSize(), savedDevice->preferredSampleRate()))
                break;
            m_dev = std::move(savedDevice);
            postSetDevice(true);
            connect(m_drv, &talcs::AudioDriver::deviceChanged, this, &OutputSystem::handleDeviceHotPlug);
            qDebug() << "Audio::OutputSystem: initialized from saved device" << m_drv->name() << m_dev->name();
            return AbstractOutputSystem::initialize();
        } while (false);
        qDebug() << "Audio::OutputSystem: no saved device, try to enumerate devices";
        if (enumerateDevices(false)) {
            qDebug() << "Audio::OutputSystem: initialized by enumerating devices" << m_drv->name() << m_dev->name();
        } else {
            qWarning() << "Audio::OutputSystem: fatal: no available device";
            return false;
        }
        return AbstractOutputSystem::initialize();
    }
    talcs::AudioDriverManager *OutputSystem::driverManager() const {
        return m_drvMgr;
    }
    talcs::AudioDriver *OutputSystem::driver() const {
        return m_drv;
    }
    talcs::AudioDevice *OutputSystem::device() const {
        return m_dev.get();
    }
    bool OutputSystem::setDriver(const QString &driverName) {
        // Note: If this function fails, m_drv will become null.
        if (m_drv && driverName == m_drv->name())
            return true;
        if (m_dev) {
            qDebug() << "Audio::OutputSystem: destroy device because driver will be reset:" << m_dev->name();
            m_dev.reset();
        }
        if (m_drv) {
            qDebug() << "Audio::OutputSystem: finalize old driver:" << m_drv->name();
            m_drv->finalize();
            disconnect(m_drv, nullptr, this, nullptr);
        }
        m_drv = m_drvMgr->driver(driverName);
        if (!m_drv || !m_drv->initialize()) {
            qWarning() << "Audio::OutputSystem: fatal: cannot find or initialize driver:" << driverName;
            m_drv = nullptr;
            return false;
        }
        qDebug() << "Audio::OutputSystem: driver set to" << driverName;
        return enumerateDevices(true);
    }
    bool OutputSystem::setDevice(const QString &deviceName) {
        // Note: If this function fails, m_dev will remain the previous one.
        std::unique_ptr<talcs::AudioDevice> currentDevice;
        currentDevice.reset(m_drv->createDevice(deviceName));
        if (!currentDevice || !currentDevice->isInitialized()) {
            qWarning() << "Audio::OutputSystem: fatal: cannot create or initialize device:" << deviceName;
            return false;
        }
        auto savedBufferSize = m_adoptedBufferSize == 0 ? currentDevice->preferredBufferSize() : m_adoptedBufferSize;
        auto savedSampleRate = qFuzzyIsNull(m_adoptedSampleRate) ? currentDevice->preferredSampleRate() : m_adoptedSampleRate;
        if (!currentDevice->open(savedBufferSize, savedSampleRate) && !currentDevice->open(currentDevice->preferredBufferSize(), currentDevice->preferredSampleRate())) {
            qWarning() << "Audio::OutputSystem: fatal: cannot open device:" << deviceName;
            return false;
        }
        m_dev = std::move(currentDevice);
        qDebug() << "Audio::OutputSystem: device set to" << deviceName;
        postSetDevice(true);
        return true;
    }
    bool OutputSystem::enumerateDevices(bool saveToSettings) {
        for (int i = 0;; i++) {
            if (m_drv && m_drv->initialize()) {
                qDebug() << "Audio::OutputSystem: driver is set, skip enumerating driver" << m_drv->name();
                break;
            }
            if (i >= m_drvMgr->drivers().size()) {
                qWarning() << "Audio::OutputSystem: fatal: all drivers are enumerated but none of them can be used";
                m_drv = nullptr;
                return false;
            }
            m_drv = m_drvMgr->driver(m_drvMgr->drivers()[i]);
            if (m_drv && m_drv->initialize()) {
                qDebug() << "Audio::OutputSystem: driver found by enumerating" << m_drv->name();
                break;
            }
        }
        for (int i = -1;; i++) {
            std::unique_ptr<talcs::AudioDevice> currentDevice;
            if (i >= m_drv->devices().size()) {
                qWarning() << "Audio::OutputSystem: fatal: all devices are enumerated but none of them can be used";
                return false;
            }
            if (i == -1) {
                if (!m_drv->defaultDevice().isEmpty())
                    currentDevice.reset(m_drv->createDevice(m_drv->defaultDevice()));
            } else {
                currentDevice.reset(m_drv->createDevice(m_drv->devices()[i]));
            }
            if (!currentDevice || !currentDevice->isInitialized())
                continue;
            auto savedBufferSize = m_adoptedBufferSize == 0 ? currentDevice->preferredBufferSize() : m_adoptedBufferSize;
            auto savedSampleRate = qFuzzyIsNull(m_adoptedSampleRate) ? currentDevice->preferredSampleRate() : m_adoptedSampleRate;
            if (!currentDevice->open(savedBufferSize, savedSampleRate) && !currentDevice->open(currentDevice->preferredBufferSize(), currentDevice->preferredSampleRate()))
                continue;
            m_dev = std::move(currentDevice);
            qDebug() << "Audio::OutputSystem: device found by enumerating" << m_dev->name();
            postSetDevice(saveToSettings);
            connect(m_drv, &talcs::AudioDriver::deviceChanged, this, &OutputSystem::handleDeviceHotPlug);
            return true;
        }
    }
    qint64 OutputSystem::adoptedBufferSize() const {
        return m_adoptedBufferSize;
    }
    void OutputSystem::setAdoptedBufferSize(qint64 bufferSize) {
        auto &settings = *Core::ILoader::instance()->settings();
        if (bufferSize == m_adoptedBufferSize)
            return;
        if (m_dev && m_dev->isOpen())
            m_dev->open(bufferSize, m_adoptedSampleRate);
        m_adoptedBufferSize = bufferSize;
        emit bufferSizeChanged(bufferSize);
        auto obj = settings["Audio"].toObject();
        obj["adoptedBufferSize"] = m_adoptedBufferSize;
        settings["Audio"] = obj;
        m_deviceControlMixer->open(m_adoptedBufferSize, m_adoptedSampleRate);
    }
    double OutputSystem::adoptedSampleRate() const {
        return m_adoptedSampleRate;
    }
    void OutputSystem::setAdoptedSampleRate(double sampleRate) {
        auto &settings = *Core::ILoader::instance()->settings();
        if (qFuzzyCompare(sampleRate, m_adoptedSampleRate))
            return;
        if (m_dev && m_dev->isOpen())
            m_dev->open(m_adoptedBufferSize, sampleRate);
        m_adoptedSampleRate = sampleRate;
        emit sampleRateChanged(sampleRate);
        auto obj = settings["Audio"].toObject();
        obj["adoptedSampleRate"] = m_adoptedSampleRate;
        settings["Audio"] = obj;
        m_deviceControlMixer->open(m_adoptedBufferSize, m_adoptedSampleRate);
    }
    bool OutputSystem::makeReady() {
        if (!m_dev) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because device is null";
            return false;
        }
        if (!m_adoptedBufferSize || qFuzzyIsNull(m_adoptedSampleRate)) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because buffer size or sample rate is null";
            return false;
        }
        if (!m_dev->isOpen() && !m_dev->open(m_adoptedBufferSize, m_adoptedSampleRate)) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because cannot open audio device";
            return false;
        }
        if (!m_deviceControlMixer->isOpen() && !m_deviceControlMixer->open(m_adoptedBufferSize, m_adoptedSampleRate)) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because cannot open pre-mixer";
            return false;
        }
        if (!m_dev->isStarted() && !m_dev->start(m_playback.get())) {
            qWarning() << "Audio::OutputSystem: fatal: cannot make ready because cannot start audio device";
            return false;
        }
        return true;
    }
    QString OutputSystem::driverDisplayName(const QString &driverName) {
        return SDLAudioDeviceDisplayNameHelper::getDisplayNameOf(driverName);
    }
    OutputSystem::HotPlugNotificationMode OutputSystem::hotPlugNotificationMode() const {
        return m_hotPlugNotificationMode;
    }
    void OutputSystem::setHotPlugNotificationMode(OutputSystem::HotPlugNotificationMode mode) {
        m_hotPlugNotificationMode = mode;
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["hotPlugNotificationMode"] = mode;
        settings["Audio"] = obj;
        qDebug() << "Audio::OutputSystem: hot plug notification mode set to" << mode;
    }
    void OutputSystem::setFileBufferingMsec(double msec) {
        setAdoptedBufferSize(static_cast<qint64>(msec * m_adoptedSampleRate / 1000));
    }
    void OutputSystem::handleDeviceHotPlug() {
        qDebug() << "Audio::OutputSystem: hot plug detected";
        switch (m_hotPlugNotificationMode) {
            case Omni:
                emit deviceHotPlugged();
                break;
            case Current:
                if (m_dev && !m_drv->devices().contains(m_dev->name()))
                    emit deviceHotPlugged();
                break;
            case None:
                break;
        }
    }
    void OutputSystem::postSetDevice(bool saveToSettings) {
        auto &settings = *Core::ILoader::instance()->settings();
        if (m_adoptedBufferSize != m_dev->bufferSize()) {
            m_adoptedBufferSize = m_dev->bufferSize();
            emit bufferSizeChanged(m_adoptedBufferSize);
        }
        m_adoptedBufferSize = m_dev->bufferSize();
        if (!qFuzzyCompare(m_adoptedSampleRate, m_dev->sampleRate())) {
            m_adoptedSampleRate = m_dev->sampleRate();
            emit sampleRateChanged(m_adoptedSampleRate);
        }
        m_deviceControlMixer->open(m_adoptedBufferSize, m_adoptedSampleRate);
        if (saveToSettings) {
            qDebug() << "Audio::OutputSystem: device info saved to settings" << m_drv->name() << m_dev->name() << m_adoptedBufferSize << m_adoptedSampleRate;
            auto obj = settings["Audio"].toObject();
            obj["driverName"] = m_drv->name();
            obj["deviceName"] = m_dev->name();
            obj["adoptedSampleRate"] = m_adoptedSampleRate;
            obj["adoptedBufferSize"] = m_adoptedBufferSize;
            settings["Audio"] = obj;
        }
        emit deviceChanged();
    }
}