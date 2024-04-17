#include "abstractoutputsystem.h"

#include <QDebug>

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDevice/AudioSourcePlayback.h>

#include "audiodevicetesteraudiosource.h"

namespace Audio {
    AbstractOutputSystem::AbstractOutputSystem(QObject *parent) : QObject(parent), m_addOnLoader({}, this) {
        m_preMixer = new talcs::MixerAudioSource(this);
        m_playback = std::make_unique<talcs::AudioSourcePlayback>(m_preMixer, false, false);
        m_preMixer->addSource(m_deviceTester = new AudioDeviceTesterAudioSource, true);
    }

    AbstractOutputSystem::~AbstractOutputSystem() = default;

    bool AbstractOutputSystem::initialize() {
        if (!m_addOnLoader.initializeAll()) {
            qDebug() << "Audio::OutputSystem: cannot initialize output system add-ons";
            return false;
        }
        m_addOnLoader.extensionInitializedAll();
        m_addOnLoader.delayedInitializeAll();
        return true;
    }

    talcs::MixerAudioSource *AbstractOutputSystem::preMixer() const {
        return m_preMixer;
    }
    void AbstractOutputSystem::testDevice() {
        m_deviceTester->playTestSound();
    }
} // Audio