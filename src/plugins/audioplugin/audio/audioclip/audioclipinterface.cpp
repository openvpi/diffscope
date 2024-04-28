#include "audioclipinterface.h"
#include "audioclipinterface_p.h"

#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/AudioSourceClipSeries.h>
#include <TalcsCore/Decibels.h>

#include <audioplugin/trackinterface.h>

namespace Audio {

    void AudioClipInterfacePrivate::handleEntityGainChange(double gainDecibel) const {
        clipControlMixer->setGain(talcs::Decibels::decibelsToGain(static_cast<float>(gainDecibel)));
    }
    void AudioClipInterfacePrivate::handleEntityPanChange(double pan) const {
        clipControlMixer->setPan(static_cast<float>(pan));
    }
    void AudioClipInterfacePrivate::handleEntityMuteChange(bool isMute) const {
        clipControlMixer->setSilentFlags(isMute ? -1 : 0);
    }

    AudioClipInterface::AudioClipInterface(QDspx::AudioClipEntity *entity, TrackInterface *track, QObject *parent) : AudioClipInterface(*new AudioClipInterfacePrivate, parent) {
        Q_D(AudioClipInterface);
        d->entity = entity;
        d->track = track;
        // TODO Clip Audio Source
        d->clipMixer = new talcs::PositionableMixerAudioSource;
        d->clipControlMixer = std::make_unique<talcs::PositionableMixerAudioSource>();
        d->clipControlMixer->addSource(d->clipMixer);
        // TODO Add to track clip series
    }
    AudioClipInterface::AudioClipInterface(AudioClipInterfacePrivate &d, QObject *parent) : Core::IExecutive(parent), d_ptr(&d) {
        d.q_ptr = this;
    }
    AudioClipInterface::~AudioClipInterface() {
        Q_D(AudioClipInterface);
        // TODO Remove from track clip series
    }
    QDspx::AudioClipEntity *AudioClipInterface::entity() const {
        Q_D(const AudioClipInterface);
        return d->entity;
    }
    TrackInterface *AudioClipInterface::track() const {
        Q_D(const AudioClipInterface);
        return d->track;
    }
    talcs::PositionableMixerAudioSource *AudioClipInterface::clipMixer() const {
        Q_D(const AudioClipInterface);
        return d->clipMixer;
    }
    talcs::PositionableAudioSource *AudioClipInterface::clipAudioSource() const {
        Q_D(const AudioClipInterface);
        return d->clipAudioSource;
    }
}