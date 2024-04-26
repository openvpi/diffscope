#include "audiocontextinterface.h"
#include "audiocontextinterface_p.h"

#include <CoreApi/iwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/internal/projectaddon.h>

namespace Audio {

    void AudioContextInterfacePrivate::init(ProjectAddOn *projectAddOn_) {
        projectAddOn = projectAddOn_;
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

    Core::IWindow *AudioContextInterface::windowHandle() const {
        Q_D(const AudioContextInterface);
        return d->projectAddOn->windowHandle();
    }

    AudioContextInterface *AudioContextInterface::get(Core::IWindow *win) {
        return static_cast<AudioContextInterface *>(win->getFirstObject("Audio.AudioContextInterface"));
    }

    Core::IExecutiveRegistry<TrackInterface> *AudioContextInterface::trackRegistry() const {
        return &d_ptr->trackRegistry;
    }

    QList<TrackInterface *> AudioContextInterface::tracks() const {
        Q_D(const AudioContextInterface);
        return d->tracks.values();
    }
    TrackInterface *AudioContextInterface::getTrack(QDspx::TrackEntity *entity) const {
        Q_D(const AudioContextInterface);
        return d->tracks.value(entity);
    }

}