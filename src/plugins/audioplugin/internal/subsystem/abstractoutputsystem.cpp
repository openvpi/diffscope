#include "abstractoutputsystem.h"

#include <QDebug>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioSourcePlayback.h>
#include <TalcsDevice/AudioDevice.h>

namespace Audio::Internal {
    AbstractOutputSystem::AbstractOutputSystem(QObject *parent) : QObject(parent) {
        m_deviceControlMixer = new talcs::MixerAudioSource;
        m_preMixer = new talcs::MixerAudioSource;
        m_deviceControlMixer->addSource(m_preMixer, true);
        m_playback = std::make_unique<talcs::AudioSourcePlayback>(m_deviceControlMixer, true, false);
    }

    AbstractOutputSystem::~AbstractOutputSystem() = default;

    bool AbstractOutputSystem::initialize() {
        return true;
    }

    talcs::MixerAudioSource *AbstractOutputSystem::preMixer() const {
        return m_preMixer;
    }
    void AbstractOutputSystem::setGain(float gain) {
        m_deviceControlMixer->setGain(gain);
    }
    void AbstractOutputSystem::setPan(float pan) {
        m_deviceControlMixer->setPan(pan);
    }
    float AbstractOutputSystem::gain() const {
        return m_deviceControlMixer->gain();
    }
    float AbstractOutputSystem::pan() const {
        return m_deviceControlMixer->pan();
    }

    void AbstractOutputSystem::setFileBufferingReadAheadSize(qint64 size) {
        if (m_fileBufferingReadAheadSize != size) {
            m_fileBufferingReadAheadSize = size;
            emit fileBufferingReadAheadSizeChanged(size);
        }
    }
    qint64 AbstractOutputSystem::fileBufferingReadAheadSize() const {
        return m_fileBufferingReadAheadSize;
    }

}