#include "outputsystem.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDevice/AudioDriver.h>
#include <TalcsDevice/AudioDriverManager.h>
#include <TalcsDevice/AudioSourcePlayback.h>

#include "icore.h"
#include "CoreApi/iloader.h"

namespace Audio {
    OutputSystem::OutputSystem(QObject *parent) : QObject(parent) {
        m_drvMgr = talcs::AudioDriverManager::createBuiltInDriverManager(this);
        m_preMixer = new talcs::MixerAudioSource(this);
        m_playback = std::make_unique<talcs::AudioSourcePlayback>(m_preMixer, false, false);
    }
    OutputSystem::~OutputSystem() {
    }
    bool OutputSystem::initialize() {
        if (!Core::ILoader::instance()->settings()->contains("Audio"))
            Core::ILoader::instance()->settings()->insert("Audio", QJsonObject());
        m_settingObj = std::make_unique<QJsonObject>(Core::ILoader::instance()->settings()->value("Audio").toObject());
        m_adoptedBufferSize = m_settingObj->value("adoptedBufferSize").toInt();
        m_adoptedSampleRate = m_settingObj->value("adoptedSampleRate").toDouble();
        auto savedDriverName = m_settingObj->value("driverName").toString();
        auto savedDeviceName = m_settingObj->value("deviceName").toString();
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
            return true;
        } while (false);
        return enumerateDevices(false);
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
    talcs::AudioSourcePlayback *OutputSystem::playback() const {
        return m_playback.get();
    }
    talcs::MixerAudioSource *OutputSystem::preMixer() const {
        return m_preMixer;
    }
    bool OutputSystem::setDriver(const QString &driverName) {
        if (driverName == m_drv->name())
            return true;
        m_dev.reset();
        if (m_drv) {
            m_drv->finalize();
            disconnect(m_drv, nullptr, this, nullptr);
        }
        m_drv = m_drvMgr->driver(driverName);
        if (!m_drv || !m_drv->initialize()) {
            m_drv = nullptr;
            return false;
        }
        return enumerateDevices(true);
    }
    bool OutputSystem::setDevice(const QString &deviceName) {
        std::unique_ptr<talcs::AudioDevice> currentDevice;
        currentDevice.reset(m_drv->createDevice(deviceName));
        if (!currentDevice || !currentDevice->isInitialized())
            return false;
        auto savedBufferSize = m_adoptedBufferSize == 0 ? currentDevice->preferredBufferSize() : m_adoptedBufferSize;
        auto savedSampleRate = qFuzzyIsNull(m_adoptedSampleRate) ? currentDevice->preferredSampleRate() : m_adoptedSampleRate;
        if (!currentDevice->open(savedBufferSize, savedSampleRate) && !currentDevice->open(currentDevice->preferredBufferSize(), currentDevice->preferredSampleRate()))
            return false;
        m_dev = std::move(currentDevice);
        postSetDevice(true);
        return true;
    }
    bool OutputSystem::enumerateDevices(bool saveToSettings) {
        for (int i = 0;; i++) {
            if (m_drv && m_drv->initialize())
                break;
            if (i >= m_drvMgr->drivers().size()) {
                m_drv = nullptr;
                return false;
            }
            m_drv = m_drvMgr->driver(m_drvMgr->drivers()[i]);
            if (m_drv && m_drv->initialize())
                break;
        }
        for (int i = -1;; i++) {
            std::unique_ptr<talcs::AudioDevice> currentDevice;
            if (i >= m_drv->devices().size())
                return false;
            if (i == -1 && !m_drv->defaultDevice().isEmpty())
                currentDevice.reset(m_drv->createDevice(m_drv->defaultDevice()));
            else
                currentDevice.reset(m_drv->createDevice(m_drv->devices()[i]));
            if (!currentDevice || !currentDevice->isInitialized())
                continue;
            auto savedBufferSize = m_adoptedBufferSize == 0 ? currentDevice->preferredBufferSize() : m_adoptedBufferSize;
            auto savedSampleRate = qFuzzyIsNull(m_adoptedSampleRate) ? currentDevice->preferredSampleRate() : m_adoptedSampleRate;
            if (!currentDevice->open(savedBufferSize, savedSampleRate) && !currentDevice->open(currentDevice->preferredBufferSize(), currentDevice->preferredSampleRate()))
                continue;
            m_dev = std::move(currentDevice);
            postSetDevice(saveToSettings);
            connect(m_drv, &talcs::AudioDriver::deviceChanged, this, &OutputSystem::handleDeviceHotPlug);
            return true;
        }
    }
    qint64 OutputSystem::adoptedBufferSize() const {
        return m_adoptedBufferSize;
    }
    void OutputSystem::setAdoptedBufferSize(qint64 bufferSize) {
        if (bufferSize == m_adoptedBufferSize)
            return;
        if (m_dev && m_dev->isOpen())
            m_dev->open(bufferSize, m_adoptedSampleRate);
        m_adoptedBufferSize = bufferSize;
        emit adoptedBufferSizeChanged(bufferSize);
        m_settingObj->insert("adoptedBufferSize", m_adoptedBufferSize);
        m_preMixer->open(m_adoptedBufferSize, m_adoptedSampleRate);
    }
    double OutputSystem::adoptedSampleRate() const {
        return m_adoptedSampleRate;
    }
    void OutputSystem::setAdoptedSampleRate(double sampleRate) {
        if (qFuzzyCompare(sampleRate, m_adoptedSampleRate))
            return;
        if (m_dev && m_dev->isOpen())
            m_dev->open(m_adoptedBufferSize, sampleRate);
        m_adoptedSampleRate = sampleRate;
        emit adoptedSampleRateChanged(sampleRate);
        m_settingObj->insert("adoptedSampleRate", m_adoptedSampleRate);
        m_preMixer->open(m_adoptedBufferSize, m_adoptedSampleRate);
    }
    void OutputSystem::testDevice() {
        // TODO
    }
    QString OutputSystem::driverDisplayName(const QString &driverName) {
        return driverName; // TODO
    }
    OutputSystem::HotPlugNotificationMode OutputSystem::hotPlugNotificationMode() const {
        return m_hotPlugNotificationMode;
    }
    void OutputSystem::setHotPlugNotificationMode(OutputSystem::HotPlugNotificationMode mode) {
        m_hotPlugNotificationMode = mode;
    }
    void OutputSystem::handleDeviceHotPlug() {
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
        m_adoptedBufferSize = m_dev->bufferSize();
        if (!qFuzzyCompare(m_adoptedSampleRate, m_dev->sampleRate())) {
            m_adoptedSampleRate = m_dev->sampleRate();
            emit adoptedSampleRateChanged(m_adoptedSampleRate);
        }
        m_preMixer->open(m_adoptedBufferSize, m_adoptedSampleRate);
        if (saveToSettings) {
            m_settingObj->insert("driverName", m_drv->name());
            m_settingObj->insert("deviceName", m_dev->name());
            m_settingObj->insert("adoptedSampleRate", m_adoptedSampleRate);
            m_settingObj->insert("adoptedBufferSize", m_adoptedBufferSize);
        }
        emit deviceChanged();

    }
} // Audio