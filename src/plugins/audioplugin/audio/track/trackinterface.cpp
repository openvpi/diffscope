#include "trackinterface.h"
#include "trackinterface_p.h"

#include <TalcsCore/AudioSourceClipSeries.h>
#include <TalcsCore/PositionableMixerAudioSource.h>

#include <audioplugin/audiocontextinterface.h>

namespace Audio {
    TrackInterface::TrackInterface(QDspx::TrackEntity *entity, AudioContextInterface *context, QObject *parent) : TrackInterface(*new TrackInterfacePrivate, parent) {
        Q_D(TrackInterface);
        d->entity = entity;
        d->context = context;
        d->clipSeries = new talcs::AudioSourceClipSeries;
        d->trackMixer = new talcs::PositionableMixerAudioSource;
        d->trackControlMixer = std::make_unique<talcs::PositionableMixerAudioSource>();
        context->masterTrackMixer()->addSource(d->trackControlMixer.get());
    }
    TrackInterface::TrackInterface(TrackInterfacePrivate &d, QObject *parent) : Core::IExecutive(parent), d_ptr(&d) {
        d.q_ptr = this;
    }
    TrackInterface::~TrackInterface() {
        Q_D(TrackInterface);
        d->context->masterTrackMixer()->removeSource(d->trackControlMixer.get());
    }
    QDspx::TrackEntity *TrackInterface::entity() const {
        Q_D(const TrackInterface);
        return d->entity;
    }
    AudioContextInterface *TrackInterface::context() const {
        Q_D(const TrackInterface);
        return d->context;
    }
    talcs::PositionableMixerAudioSource *TrackInterface::trackMixer() const {
        Q_D(const TrackInterface);
        return d->trackMixer;
    }
    talcs::AudioSourceClipSeries *TrackInterface::clipSeries() const {
        Q_D(const TrackInterface);
        return d->clipSeries;
    }
    QList<AudioClipInterface *> TrackInterface::clips() const {
        Q_D(const TrackInterface);
        return d->clips.values();
    }
    AudioClipInterface *TrackInterface::getClip(QDspx::AudioClipEntity *entity) const {
        Q_D(const TrackInterface);
        return d->clips.value(entity);
    }
}