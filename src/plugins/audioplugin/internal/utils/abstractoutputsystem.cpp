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
    void AbstractOutputSystem::setSubstitutedSource(talcs::AudioSource *source, IOutputSystemAddOn *substitutor) {
        m_substitutedSource = source;
        m_substitutor = substitutor;
        m_playback->setSource(source, false, false);
    }
    talcs::AudioSource *AbstractOutputSystem::substitutedSource() const {
        return m_substitutedSource;
    }
    IOutputSystemAddOn *AbstractOutputSystem::sourceSubstitutor() const {
        return m_substitutor;
    }
    void AbstractOutputSystem::resetSubstitutedSource() {
        m_substitutedSource = nullptr;
        m_substitutor = nullptr;
        m_playback->setSource(m_preMixer, false, false);
    }
    talcs::AudioSource *AbstractOutputSystem::currentSource() const {
        return m_substitutedSource ? m_substitutedSource : m_preMixer;
    }
} // Audio