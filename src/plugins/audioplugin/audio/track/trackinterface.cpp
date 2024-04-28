#include "trackinterface.h"
#include "trackinterface_p.h"

#include <TalcsCore/AudioSourceClipSeries.h>
#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/Decibels.h>

#include <trackentity.h>

#include <audioplugin/audiocontextinterface.h>
#include <audioplugin/audioclipinterface.h>

namespace Audio {

    void TrackInterfacePrivate::handleEntityGainChange(double gainDecibel) const {
        trackControlMixer->setGain(talcs::Decibels::decibelsToGain(static_cast<float>(gainDecibel)));
    }
    void TrackInterfacePrivate::handleEntityPanChange(double pan) const {
        trackControlMixer->setPan(static_cast<float>(pan));
    }
    void TrackInterfacePrivate::handleEntityMuteChange(bool isMuted) const {
        trackControlMixer->setSilentFlags(isMuted ? -1 : 0);
    }
    void TrackInterfacePrivate::handleEntitySoloChange(bool isSolo) const {
        context->masterTrackMixer()->setSourceSolo(trackControlMixer.get(), isSolo);
    }
    void TrackInterfacePrivate::handleClipInserted(QDspx::ClipEntity *clipEntity) {
        Q_Q(TrackInterface);
        if (clipEntity->type() == QDspx::ClipEntity::Audio) {
            auto audioClipEntity = static_cast<QDspx::AudioClipEntity *>(clipEntity);
            auto clip = context->audioClipRegistry()->create(audioClipEntity, q, q);
            clips.insert(audioClipEntity, clip);
        }
    }
    void TrackInterfacePrivate::handleClipAboutToRemove(QDspx::ClipEntity *clipEntity) {
        Q_Q(TrackInterface);
        if (clipEntity->type() == QDspx::ClipEntity::Audio) {
            auto audioClipEntity = static_cast<QDspx::AudioClipEntity *>(clipEntity);
            auto clip = clips.value(audioClipEntity);
            if (clip) {
                clip->quit();
                delete clip;
                clips.remove(audioClipEntity);
            }
        }
    }

    TrackInterface::TrackInterface(QDspx::TrackEntity *entity, AudioContextInterface *context, QObject *parent) : TrackInterface(*new TrackInterfacePrivate, parent) {
        Q_D(TrackInterface);
        d->entity = entity;
        d->context = context;
        d->clipSeries = new talcs::AudioSourceClipSeries;
        d->trackMixer = new talcs::PositionableMixerAudioSource;
        d->trackMixer->addSource(d->clipSeries);
        d->trackControlMixer = std::make_unique<talcs::PositionableMixerAudioSource>();
        d->trackControlMixer->addSource(d->trackMixer);

        connect(entity->control(), &QDspx::TrackControlEntity::gainChanged, this, [=](double gainDecibel) {
            d->handleEntityGainChange(gainDecibel);
        });
        connect(entity->control(), &QDspx::TrackControlEntity::panChanged, this, [=](double pan) {
            d->handleEntityPanChange(pan);
        });
        connect(entity->control(), &QDspx::TrackControlEntity::muteChanged, this, [=](bool isMuted) {
            d->handleEntityMuteChange(isMuted);
        });
        connect(entity->control(), &QDspx::TrackControlEntity::soloChanged, this, [=](bool isSolo) {
            d->handleEntitySoloChange(isSolo);
        });
        connect(entity->clips(), &QDspx::ClipListEntity::inserted, this, [=](int id, QDspx::ClipEntity *clipEntity) {
            d->handleClipInserted(clipEntity);
        });
        connect(entity->clips(), &QDspx::ClipListEntity::aboutToRemove, this, [=](int id, QDspx::ClipEntity *clipEntity) {
            d->handleClipAboutToRemove(clipEntity);
        });
        d->handleEntityGainChange(entity->control()->gain());
        d->handleEntityPanChange(entity->control()->pan());
        d->handleEntityMuteChange(entity->control()->mute());
        d->handleEntitySoloChange(entity->control()->solo());
        for (int id : entity->clips()->ids()) {
            d->handleClipInserted(entity->clips()->value(id));
        }

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