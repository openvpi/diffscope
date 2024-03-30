#include "abstractoutputsystem.h"

#include <QDebug>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioSourcePlayback.h>

#include "audiodevicetesteraudiosource.h"

namespace Audio {
    AbstractOutputSystem::AbstractOutputSystem(QObject *parent) : QObject(parent) {
        m_preMixer = new talcs::MixerAudioSource(this);
        m_playback = std::make_unique<talcs::AudioSourcePlayback>(m_preMixer, false, false);
        m_preMixer->addSource(m_deviceTester = new AudioDeviceTesterAudioSource, true);
    }

    AbstractOutputSystem::~AbstractOutputSystem() = default;

    talcs::MixerAudioSource *AbstractOutputSystem::preMixer() const {
        return m_preMixer;
    }
    void AbstractOutputSystem::testDevice() {
        m_deviceTester->playTestSound();
    }
} // Audio