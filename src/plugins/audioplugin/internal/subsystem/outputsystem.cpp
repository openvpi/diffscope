#include "outputsystem.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDevice/AudioDriver.h>
#include <TalcsDevice/AudioDriverManager.h>
#include <TalcsDevice/AudioSourcePlayback.h>

#include <icore.h>
#include <CoreApi/iloader.h>

#include <audioplugin/internal/sdlaudiodevicedisplaynamehelper.h>

namespace Audio::Internal {
    OutputSystem::OutputSystem(QObject *parent) : AbstractOutputSystem(parent), m_outputContext(new talcs::OutputContext) {
        setContext(m_outputContext.get());
    }

    OutputSystem::~OutputSystem() = default;

    bool OutputSystem::initialize() {
        const auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();

        m_outputContext->setAdoptedBufferSize(obj["adoptedBufferSize"].toInt());
        m_outputContext->setAdoptedSampleRate(obj["adoptedSampleRate"].toDouble());
        m_outputContext->controlMixer()->setGain(static_cast<float>(obj["deviceGain"].toDouble(1.0)));
        m_outputContext->controlMixer()->setPan(static_cast<float>(obj["devicePan"].toDouble()));
        setFileBufferingReadAheadSize(obj["fileBufferingReadAheadSize"].toInt());

        if (m_outputContext->initialize(obj["driverName"].toString(), obj["deviceName"].toString())) {
            qDebug() << "Audio::OutputSystem: device initialized"
                     << m_outputContext->device()->name()
                     << m_outputContext->driver()->name()
                     << m_outputContext->adoptedBufferSize()
                     << m_outputContext->adoptedSampleRate();
            return true;
        } else {
            qWarning() << "Audio::OutputSystem: fatal: cannot initialize";
            return false;
        }

    }
    bool OutputSystem::setDriver(const QString &driverName) {
        if (m_outputContext->setDriver(driverName)) {
            postSetDevice();
            qDebug() << "Audio::OutputSystem: driver changed"
                     << m_outputContext->device()->name()
                     << m_outputContext->driver()->name()
                     << m_outputContext->adoptedBufferSize()
                     << m_outputContext->adoptedSampleRate();
            return true;
        } else {
            qWarning() << "Audio::OutputSystem: fatal: cannot set driver" << driverName;
            return false;
        }
    }
    bool OutputSystem::setDevice(const QString &deviceName) {
        if (m_outputContext->setDevice(deviceName)) {
            postSetDevice();
            qDebug() << "Audio::OutputSystem: device changed"
                     << m_outputContext->device()->name()
                     << m_outputContext->driver()->name()
                     << m_outputContext->adoptedBufferSize()
                     << m_outputContext->adoptedSampleRate();
            return true;
        } else {
            qWarning() << "Audio::OutputSystem: fatal: cannot set device"
                       << (m_outputContext->driver() ? m_outputContext->driver()->name() : "")
                       << deviceName;
            return false;
        }
    }

    bool OutputSystem::setAdoptedBufferSize(qint64 bufferSize) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["adoptedBufferSize"] = bufferSize;
        settings["Audio"] = obj;
        if (m_outputContext->setAdoptedBufferSize(bufferSize)) {
            qDebug() << "Audio::OutputSystem: buffer size changed"
                     << m_outputContext->device()->name()
                     << m_outputContext->driver()->name()
                     << m_outputContext->adoptedBufferSize()
                     << m_outputContext->adoptedSampleRate();
            return true;
        } else {
            qWarning() << "Audio::OutputSystem: fatal: cannot set buffer size"
                     << (m_outputContext->driver() ? m_outputContext->driver()->name() : "")
                     << (m_outputContext->device() ? m_outputContext->device()->name() : "")
                     << m_outputContext->adoptedBufferSize()
                     << m_outputContext->adoptedSampleRate();
            return false;
        }
    }
    
    bool OutputSystem::setAdoptedSampleRate(double sampleRate) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["adoptedSampleRate"] = sampleRate;
        settings["Audio"] = obj;
        if (m_outputContext->setAdoptedSampleRate(sampleRate)) {
            qDebug() << "Audio::OutputSystem: sample rate changed"
                     << m_outputContext->device()->name()
                     << m_outputContext->driver()->name()
                     << m_outputContext->adoptedSampleRate()
                     << m_outputContext->adoptedSampleRate();
            return true;
        } else {
            qWarning() << "Audio::OutputSystem: fatal: cannot set sample rate"
                       << (m_outputContext->driver() ? m_outputContext->driver()->name() : "")
                       << (m_outputContext->device() ? m_outputContext->device()->name() : "")
                       << m_outputContext->adoptedSampleRate()
                       << m_outputContext->adoptedSampleRate();
            return false;
        }
    }

    void OutputSystem::setHotPlugNotificationMode(talcs::OutputContext::HotPlugNotificationMode mode) {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["hotPlugNotificationMode"] = mode;
        settings["Audio"] = obj;
        m_outputContext->setHotPlugNotificationMode(mode);
        qDebug() << "Audio::OutputSystem: hot plug notification mode set to" << mode;
    }

    void OutputSystem::postSetDevice() const {
        auto &settings = *Core::ILoader::instance()->settings();
        auto obj = settings["Audio"].toObject();
        obj["driverName"] = m_outputContext->driver()->name();
        obj["deviceName"] = m_outputContext->device()->name();
        obj["adoptedSampleRate"] = m_outputContext->adoptedSampleRate();
        obj["adoptedBufferSize"] = m_outputContext->adoptedBufferSize();
        settings["Audio"] = obj;
    }
}