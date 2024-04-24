#include "audiocontextinterface.h"
#include "audiocontextinterface_p.h"

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/iaudiocontextaddon.h>
#include <audioplugin/internal/projectaddon.h>
#include <audioplugin/private/iaudiocontextaddon_p.h>

namespace Audio {

    void AudioContextInterfacePrivate::init(ProjectAddOn *projectAddOn_) {
        projectAddOn = projectAddOn_;
    }

    void AudioContextInterfacePrivate::initializeAddOns() {
        Q_Q(AudioContextInterface);
        addOnLoader = std::make_unique<AddOnLoader<IAudioContextAddOn>>(IAudio::instance()->audioContextAddOns(), q);
        for (auto addOn : addOnLoader->addOns()) {
            addOn->d_func()->audioContextInterface = q;
        }
        addOnLoader->initializeAll();
        addOnLoader->extensionInitializedAll();
        addOnLoader->delayedInitializeAll();
    }

    AudioContextInterface::AudioContextInterface(QObject *parent) : QObject(parent), d_ptr(new AudioContextInterfacePrivate) {
        Q_D(AudioContextInterface);
        d->q_ptr = this;
    }

    AudioContextInterface::~AudioContextInterface() = default;

    talcs::MixerAudioSource *AudioContextInterface::preMixer() const {
        Q_D(const AudioContextInterface);
        return d->projectAddOn->preMixer();
    }

    talcs::TransportAudioSource *AudioContextInterface::transportAudioSource() const {
        Q_D(const AudioContextInterface);
        return d->projectAddOn->transportAudioSource();
    }

    talcs::PositionableMixerAudioSource *AudioContextInterface::postMixer() const {
        Q_D(const AudioContextInterface);
        return d->projectAddOn->postMixer();
    }

    talcs::PositionableMixerAudioSource *AudioContextInterface::masterTrackMixer() const {
        Q_D(const AudioContextInterface);
        return d->projectAddOn->masterTrackMixer();
    }

    talcs::PositionableAudioSource *AudioContextInterface::substitutedSource() const {
        Q_D(const AudioContextInterface);
        return d->projectAddOn->substitutedSource();
    }

    Core::IProjectWindow *AudioContextInterface::windowHandle() const {
        Q_D(const AudioContextInterface);
        return static_cast<Core::IProjectWindow *>(d->projectAddOn->windowHandle());
    }
} // Audio