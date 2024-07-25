#include "trackinterface.h"
#include "trackinterface_p.h"

#include <TalcsCore/AudioSourceClipSeries.h>
#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDspx/DspxTrackContext.h>

#include <trackentity.h>

#include <audioplugin/audiocontextinterface.h>
#include <audioplugin/audioclipinterface.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/audiocontextinterface_p.h>

#define DEVICE_LOCKER talcs::AudioDeviceLocker locker(d->audioContextInterface->outputSystemInterface()->device())

namespace Audio {

    void TrackInterfacePrivate::handleGainChanged(double gainDecibel) const {
        trackContext->controlMixer()->setGain(talcs::Decibels::decibelsToGain(gainDecibel));
    }
    void TrackInterfacePrivate::handlePanChanged(double pan) const {
        trackContext->controlMixer()->setPan(static_cast<float>(pan));
    }
    void TrackInterfacePrivate::handleMuteChanged(bool isMuted) const {
        trackContext->controlMixer()->setSilentFlags(isMuted ? -1 : 0);
    }
    void TrackInterfacePrivate::handleSoloChanged(bool isSolo) const {
        audioContextInterface->masterTrackMixer()->setSourceSolo(trackContext->controlMixer(), isSolo);
    }
    void TrackInterfacePrivate::handleAudioClipInserted(int id, QDspx::AudioClipEntity *audioClipEntity) {
        Q_Q(TrackInterface);
        auto audioClipContext = trackContext->addAudioClip(id);
        auto audioClipInterface = audioContextInterface->audioClipRegistry()->create(audioClipEntity, q, audioClipContext);
        clips.insert(audioClipEntity, audioClipInterface);
        audioContextInterface->d_func()->clips.insert(audioClipEntity, audioClipInterface);
    }
    void TrackInterfacePrivate::handleAudioClipAboutToRemove(int id, QDspx::AudioClipEntity *audioClipEntity) {
        Q_Q(TrackInterface);
        auto audioClipInterface = clips.value(audioClipEntity);
        if (audioClipInterface) {
            audioClipInterface->quit();
            delete audioClipInterface;
            clips.remove(audioClipEntity);
            audioContextInterface->d_func()->clips.remove(audioClipEntity);
            trackContext->removeAudioClip(id);
        }
    }

    TrackInterface::TrackInterface(QDspx::TrackEntity *entity, AudioContextInterface *audioContextInterface, talcs::DspxTrackContext *trackContext)
        : TrackInterface(*new TrackInterfacePrivate, audioContextInterface) {
        Q_D(TrackInterface);
        d->entity = entity;
        d->audioContextInterface = audioContextInterface;

        connect(entity->control(), &QDspx::TrackControlEntity::gainChanged, this, [=](double gainDecibel) {
            d->handleGainChanged(gainDecibel);
        });
        connect(entity->control(), &QDspx::TrackControlEntity::panChanged, this, [=](double pan) {
            d->handlePanChanged(pan);
        });
        connect(entity->control(), &QDspx::TrackControlEntity::muteChanged, this, [=](bool isMuted) {
            d->handleMuteChanged(isMuted);
        });
        connect(entity->control(), &QDspx::TrackControlEntity::soloChanged, this, [=](bool isSolo) {
            d->handleSoloChanged(isSolo);
        });
        connect(entity->clips(), &QDspx::ClipListEntity::inserted, this, [=](int id, QDspx::ClipEntity *clipEntity) {
            DEVICE_LOCKER;
            if (clipEntity->type() == QDspx::ClipEntity::Audio)
                d->handleAudioClipInserted(id, static_cast<QDspx::AudioClipEntity *>(clipEntity));
        });
        connect(entity->clips(), &QDspx::ClipListEntity::aboutToRemove, this, [=](int id, QDspx::ClipEntity *clipEntity) {
            DEVICE_LOCKER;
            if (clipEntity->type() == QDspx::ClipEntity::Audio)
                d->handleAudioClipAboutToRemove(id, static_cast<QDspx::AudioClipEntity *>(clipEntity));
        });

        d->handleGainChanged(entity->control()->gain());
        d->handlePanChanged(entity->control()->pan());
        d->handleMuteChanged(entity->control()->mute());
        d->handleSoloChanged(entity->control()->solo());
        for (int id : entity->clips()->ids()) {
            auto clipEntity = entity->clips()->value(id);
            if (clipEntity->type() == QDspx::ClipEntity::Audio)
                d->handleAudioClipAboutToRemove(id, static_cast<QDspx::AudioClipEntity *>(clipEntity));
        }
    }
    TrackInterface::TrackInterface(TrackInterfacePrivate &d, QObject *parent) : Core::IExecutive(parent), d_ptr(&d) {
        d.q_ptr = this;
    }
    TrackInterface::~TrackInterface() = default;

    QDspx::TrackEntity *TrackInterface::entity() const {
        Q_D(const TrackInterface);
        return d->entity;
    }
    AudioContextInterface *TrackInterface::audioContextInterface() const {
        Q_D(const TrackInterface);
        return d->audioContextInterface;
    }
    talcs::PositionableMixerAudioSource *TrackInterface::trackMixer() const {
        Q_D(const TrackInterface);
        return d->trackContext->trackMixer();
    }
    talcs::AudioSourceClipSeries *TrackInterface::clipSeries() const {
        Q_D(const TrackInterface);
        return d->trackContext->clipSeries();
    }
    QList<AudioClipInterface *> TrackInterface::clips() const {
        Q_D(const TrackInterface);
        return d->clips.values();
    }
    AudioClipInterface *TrackInterface::getAudioClipInterfaceFromEntity(QDspx::AudioClipEntity *entity) const {
        Q_D(const TrackInterface);
        return d->clips.value(entity);
    }
}