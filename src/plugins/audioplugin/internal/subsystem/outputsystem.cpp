#include "outputsystem.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDevice/AudioDriver.h>
#include <TalcsDevice/AudioDriverManager.h>
#include <TalcsDevice/AudioSourcePlayback.h>

#include <audioplugin/internal/audiosettings.h>

namespace Audio::Internal {
    OutputSystem::OutputSystem(QObject *parent) : AbstractOutputSystem(parent), m_outputContext(new talcs::OutputContext) {
        setContext(m_outputContext.get());
    }

    OutputSystem::~OutputSystem() = default;

    bool OutputSystem::initialize() {
        m_outputContext->setAdoptedBufferSize(AudioSettings::adoptedBufferSize());
        m_outputContext->setAdoptedSampleRate(AudioSettings::adoptedSampleRate());
        m_outputContext->controlMixer()->setGain(static_cast<float>(AudioSettings::deviceGain()));
        m_outputContext->controlMixer()->setPan(static_cast<float>(AudioSettings::devicePan()));
        m_outputContext->setHotPlugNotificationMode(static_cast<talcs::OutputContext::HotPlugNotificationMode>(AudioSettings::hotPlugNotificationMode()));
        setFileBufferingReadAheadSize(AudioSettings::fileBufferingReadAheadSize());

        if (m_outputContext->initialize(AudioSettings::driverName(), AudioSettings::deviceName())) {
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
        AudioSettings::setAdoptedBufferSize(bufferSize);
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
        AudioSettings::setAdoptedSampleRate(sampleRate);
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
        AudioSettings::setHotPlugNotificationMode(mode);
        m_outputContext->setHotPlugNotificationMode(mode);
        qDebug() << "Audio::OutputSystem: hot plug notification mode set to" << mode;
    }

    void OutputSystem::postSetDevice() const {
        AudioSettings::setDriverName(m_outputContext->driver()->name());
        AudioSettings::setDeviceName(m_outputContext->device()->name());
        AudioSettings::setAdoptedSampleRate(m_outputContext->adoptedSampleRate());
        AudioSettings::setAdoptedBufferSize(m_outputContext->adoptedBufferSize());
    }
}