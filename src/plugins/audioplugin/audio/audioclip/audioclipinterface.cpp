#include "audioclipinterface.h"
#include "audioclipinterface_p.h"

#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/AudioSourceClipSeries.h>
#include <TalcsCore/BufferingAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDspx/DspxAudioClipContext.h>

#include <trackentity.h>

#include <audioplugin/trackinterface.h>
#include <audioplugin/audiocontextinterface.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/audiocontextinterface_p.h>
#include <audioplugin/internal/projectaddon.h>

#define DEVICE_LOCKER talcs::AudioDeviceLocker locker(trackInterface->audioContextInterface()->outputSystemInterface()->device())

namespace Audio {

    void AudioClipInterfacePrivate::handleGainChanged(double gainDecibel) const {
        audioClipContext->controlMixer()->setGain(talcs::Decibels::decibelsToGain(gainDecibel));
    }
    void AudioClipInterfacePrivate::handlePanChanged(double pan) const {
        audioClipContext->controlMixer()->setPan(static_cast<float>(pan));
    }
    void AudioClipInterfacePrivate::handleMuteChanged(bool isMute) const {
        audioClipContext->controlMixer()->setSilentFlags(isMute ? -1 : 0);
    }
    void AudioClipInterfacePrivate::handleStartChanged(int start) const {
        audioClipContext->setStart(start);
    }
    void AudioClipInterfacePrivate::handleClipStartChanged(int clipStart) const {
        audioClipContext->setClipStart(clipStart);
    }
    void AudioClipInterfacePrivate::handleClipLenChanged(int clipLen) const {
        audioClipContext->setClipLen(clipLen);
    }
    void AudioClipInterfacePrivate::handlePathChanged(const QString &path) const {
        auto projectAddOn = trackInterface->audioContextInterface()->d_func()->projectAddOn;
        if (QString selectedFilter; projectAddOn->checkAudioClipIsToOpenFile(entity, selectedFilter)) {
            QVariant data;
            if (audioClipContext->setPathOpen(path, selectedFilter, data, projectAddOn->windowHandle()->window())) {
                // TODO entity->workspace()->insert("...", data)
            } else {
                projectAddOn->addFailedAudioClipToAlert(entity);
            }
        } else {
            QVariant data /* TODO = entity->workspace()->value("...") */;
            if (!audioClipContext->setPathLoad(path, data))
                projectAddOn->addFailedAudioClipToAlert(entity);
        }
    }

    AudioClipInterface::AudioClipInterface(QDspx::AudioClipEntity *entity, TrackInterface *trackInterface, talcs::DspxAudioClipContext *audioClipContext) : AudioClipInterface(*new AudioClipInterfacePrivate, trackInterface) {
        Q_D(AudioClipInterface);
        d->entity = entity;
        d->trackInterface = trackInterface;

        connect(entity->control(), &QDspx::BusControlEntity::gainChanged, this, [=](double gainDecibel) {
            d->handleGainChanged(gainDecibel);
        });
        connect(entity->control(), &QDspx::BusControlEntity::panChanged, this, [=](double pan) {
            d->handlePanChanged(pan);
        });
        connect(entity->control(), &QDspx::BusControlEntity::muteChanged, this, [=](bool isMuted) {
            d->handleMuteChanged(isMuted);
        });

        connect(entity->time(), &QDspx::ClipTimeEntity::startChanged, this, [=](int start) {
            DEVICE_LOCKER;
            d->handleStartChanged(start);
        });
        connect(entity->time(), &QDspx::ClipTimeEntity::clipStartChanged, this, [=](int clipStart) {
            DEVICE_LOCKER;
            d->handleClipStartChanged(clipStart);
        });
        connect(entity->time(), &QDspx::ClipTimeEntity::clipLengthChanged, this, [=](int clipLen) {
            DEVICE_LOCKER;
            d->handleClipLenChanged(clipLen);
        });
        connect(entity, &QDspx::AudioClipEntity::pathChanged, this, [=](const QString &path) {
            DEVICE_LOCKER;
            d->handlePathChanged(path);
        });

        d->handleGainChanged(entity->control()->gain());
        d->handlePanChanged(entity->control()->pan());
        d->handleMuteChanged(entity->control()->mute());
        d->handleStartChanged(entity->time()->start());
        d->handleClipStartChanged(entity->time()->clipStart());
        d->handleClipLenChanged(entity->time()->clipLength());
        d->handlePathChanged(entity->path());
    }
    AudioClipInterface::AudioClipInterface(AudioClipInterfacePrivate &d, QObject *parent) : Core::IExecutive(parent), d_ptr(&d) {
        d.q_ptr = this;
    }
    AudioClipInterface::~AudioClipInterface() = default;
    QDspx::AudioClipEntity *AudioClipInterface::entity() const {
        Q_D(const AudioClipInterface);
        return d->entity;
    }
    TrackInterface *AudioClipInterface::trackInterface() const {
        Q_D(const AudioClipInterface);
        return d->trackInterface;
    }
    talcs::PositionableMixerAudioSource *AudioClipInterface::clipMixer() const {
        Q_D(const AudioClipInterface);
        return d->audioClipContext->clipMixer();
    }
    talcs::PositionableAudioSource *AudioClipInterface::contentSource() const {
        Q_D(const AudioClipInterface);
        return d->audioClipContext->contentSource();
    }
}