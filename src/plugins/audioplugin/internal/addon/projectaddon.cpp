#include "projectaddon.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/TransportAudioSource.h>

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/audiocontextinterface_p.h>

namespace Audio {

    ProjectAddOn::ProjectAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
        m_masterTrackMixer = std::make_unique<talcs::PositionableMixerAudioSource>();
        m_masterTrackControlMixer = new talcs::PositionableMixerAudioSource;
        m_masterTrackControlMixer->addSource(m_masterTrackMixer.get());
        m_postMixer = new talcs::PositionableMixerAudioSource;
        m_postMixer->addSource(m_masterTrackControlMixer, true);
        m_tpSrc = new talcs::TransportAudioSource(m_postMixer, true);
        m_preMixer = std::make_unique<talcs::MixerAudioSource>();
        m_preMixer->addSource(m_tpSrc, true);

        m_audioContextInterface = new AudioContextInterface(this);
        m_audioContextInterface->d_func()->init(this);
    }

    ProjectAddOn::~ProjectAddOn() {
        auto iAudio = IAudio::instance();
        if (isVST()) {
            iAudio->vstOutputSystemInterface()->preMixer()->removeSource(m_preMixer.get());
        } else {
            iAudio->outputSystemInterface()->preMixer()->removeSource(m_preMixer.get());
        }
        m_tpSrc->setSource(nullptr);
    }

    void ProjectAddOn::initialize() {
        auto iAudio = IAudio::instance();
        if (isVST()) {
            iAudio->vstOutputSystemInterface()->preMixer()->addSource(m_preMixer.get());
        } else {
            iAudio->outputSystemInterface()->preMixer()->addSource(m_preMixer.get());
        }
        windowHandle()->addObject("Audio.AudioContextInterface", m_audioContextInterface);
    }

    void ProjectAddOn::extensionsInitialized() {

    }

    bool ProjectAddOn::delayedInitialize() {
        return false;
    }

    bool ProjectAddOn::isVST() const {
        // TODO
        return false;
    }

    talcs::MixerAudioSource *ProjectAddOn::preMixer() const {
        return m_preMixer.get();
    }

    talcs::TransportAudioSource *ProjectAddOn::transportAudioSource() const {
        return m_tpSrc;
    }

    talcs::PositionableMixerAudioSource *ProjectAddOn::postMixer() const {
        return m_postMixer;
    }

    talcs::PositionableMixerAudioSource *ProjectAddOn::masterTrackMixer() const {
        return m_masterTrackMixer.get();
    }

}