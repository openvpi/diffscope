#include "abstractoutputsystem.h"

#include <QDebug>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioSourcePlayback.h>
#include <TalcsDevice/AudioDevice.h>

namespace Audio {
    AbstractOutputSystem::AbstractOutputSystem(QObject *parent) : QObject(parent) {
        m_preMixer = new talcs::MixerAudioSource(this);
        m_playback = std::make_unique<talcs::AudioSourcePlayback>(m_preMixer, false, false);
    }

    AbstractOutputSystem::~AbstractOutputSystem() = default;

    bool AbstractOutputSystem::initialize() {
        return true;
    }

    talcs::MixerAudioSource *AbstractOutputSystem::preMixer() const {
        return m_preMixer;
    }
    void AbstractOutputSystem::setGainAndPan(float gain, float pan) {
        m_deviceGain = gain;
        m_devicePan = pan;
        m_preMixer->setGain(gain);
        m_preMixer->setPan(pan);
    }
    float AbstractOutputSystem::gain() const {
        return m_deviceGain;
    }
    float AbstractOutputSystem::pan() const {
        return m_devicePan;
    }

}