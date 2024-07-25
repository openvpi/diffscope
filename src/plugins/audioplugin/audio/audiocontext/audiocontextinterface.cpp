#include "audiocontextinterface.h"
#include "audiocontextinterface_p.h"

#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsFormat/FormatManager.h>
#include <TalcsDevice/AudioDevice.h>
#include <TalcsDspx/DspxProjectContext.h>
#include <TalcsDspx/DspxTrackContext.h>

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/internal/projectaddon.h>
#include <audioplugin/outputsysteminterface.h>

#define DEVICE_LOCKER talcs::AudioDeviceLocker locker(IAudio::instance()->outputSystemInterface(projectAddOn->isVST())->device())

namespace Audio {

    using namespace Internal;

    void AudioContextInterfacePrivate::init(ProjectAddOn *projectAddOn_) {
        Q_Q(AudioContextInterface);
        projectAddOn = projectAddOn_;
        projectContext = projectAddOn->projectContext();

        auto model = q->windowHandle()->doc()->dataModel().model();
        QObject::connect(model->master()->control(), &QDspx::BusControlEntity::gainChanged, q, [=] (double gainDecibel) {
            handleGainChanged(gainDecibel);
        });
        QObject::connect(model->master()->control(), &QDspx::BusControlEntity::panChanged, q, [=] (double pan) {
            handlePanChanged(pan);
        });
        QObject::connect(model->master()->control(), &QDspx::BusControlEntity::muteChanged, q, [=] (bool isMuted) {
            handleMuteChanged(isMuted);
        });
        QObject::connect(model->tracks(), &QDspx::TrackListEntity::inserted, q, [=](int index, const QVector<QDspx::TrackEntity *> &trackEntities) {
            DEVICE_LOCKER;
            for (auto trackEntity : trackEntities) {
                handleTrackInserted(index++, trackEntity);
            }
        });
        QObject::connect(model->tracks(), &QDspx::TrackListEntity::moved, q, [=](int index, int count, int dest) {
            DEVICE_LOCKER;
            handleTrackMoved(index, count, dest);
        });
        QObject::connect(model->tracks(), &QDspx::TrackListEntity::aboutToRemove, q, [=](int index, const QVector<QDspx::TrackEntity *> &trackEntities) {
            DEVICE_LOCKER;
            for (int i = trackEntities.size() - 1; i >= 0; i--) {
                handleTrackAboutToRemove(index + i, trackEntities.value(i));
            }
        });

        handleGainChanged(model->master()->control()->gain());
        handlePanChanged(model->master()->control()->pan());
        handleMuteChanged(model->master()->control()->mute());
        for (int i = 0; i < model->tracks()->size(); i++) {
            auto trackEntity = model->tracks()->at(i);
            handleTrackInserted(i, trackEntity);
        }


    }
    void AudioContextInterfacePrivate::handleGainChanged(double gainDecibel) const {
        projectContext->masterControlMixer()->setGain(talcs::Decibels::decibelsToGain(gainDecibel));
    }
    void AudioContextInterfacePrivate::handlePanChanged(double pan) const {
        projectContext->masterControlMixer()->setPan(static_cast<float>(pan));
    }
    void AudioContextInterfacePrivate::handleMuteChanged(bool isMuted) const {
        projectContext->masterControlMixer()->setSilentFlags(isMuted ? -1 : 0);
    }
    void AudioContextInterfacePrivate::handleTrackInserted(int index, QDspx::TrackEntity *trackEntity) {
        Q_Q(AudioContextInterface);
        auto trackContext = projectContext->addTrack(index);
        auto trackInterface = trackRegistry.create(trackEntity, q, trackContext);
        tracks.insert(trackEntity, trackInterface);
    }
    void AudioContextInterfacePrivate::handleTrackAboutToRemove(int index, QDspx::TrackEntity *trackEntity) {
        auto trackInterface = tracks.value(trackEntity);
        if (trackInterface) {
            trackInterface->quit();
            delete trackInterface;
            tracks.remove(trackEntity);
            projectContext->removeTrack(index);
        }
    }
    void AudioContextInterfacePrivate::handleTrackMoved(int index, int count, int dest) const {
        projectContext->moveTrack(index, count, dest);
    }

    AudioContextInterface::AudioContextInterface(QObject *parent) : QObject(parent), d_ptr(new AudioContextInterfacePrivate) {
        Q_D(AudioContextInterface);
        d->q_ptr = this;
    }

    AudioContextInterface::~AudioContextInterface() = default;

    talcs::MixerAudioSource *AudioContextInterface::preMixer() const {
        Q_D(const AudioContextInterface);
        return d->projectContext->preMixer();
    }

    talcs::TransportAudioSource *AudioContextInterface::transport() const {
        Q_D(const AudioContextInterface);
        return d->projectContext->transport();
    }

    talcs::PositionableMixerAudioSource *AudioContextInterface::postMixer() const {
        Q_D(const AudioContextInterface);
        return d->projectContext->postMixer();
    }

    talcs::PositionableMixerAudioSource *AudioContextInterface::masterTrackMixer() const {
        Q_D(const AudioContextInterface);
        return d->projectContext->masterTrackMixer();
    }

    Core::IProjectWindow *AudioContextInterface::windowHandle() const {
        Q_D(const AudioContextInterface);
        return static_cast<Core::IProjectWindow *>(d->projectAddOn->windowHandle());
    }

    AudioContextInterface *AudioContextInterface::get(Core::IProjectWindow *win) {
        return static_cast<AudioContextInterface *>(win->getFirstObject("Audio.AudioContextInterface"));
    }

    OutputSystemInterface *AudioContextInterface::outputSystemInterface() const {
        Q_D(const AudioContextInterface);
        return IAudio::instance()->outputSystemInterface(d->projectAddOn->isVST());
    }

    Core::IExecutiveRegistry<TrackInterface> *AudioContextInterface::trackRegistry() const {
        return &d_ptr->trackRegistry;
    }

    Core::IExecutiveRegistry<AudioClipInterface> *AudioContextInterface::audioClipRegistry() const {
        return &d_ptr->audioClipRegistry;
    }

    QList<TrackInterface *> AudioContextInterface::tracks() const {
        Q_D(const AudioContextInterface);
        return d->tracks.values();
    }
    TrackInterface *AudioContextInterface::getTrackInterfaceFromEntity(QDspx::TrackEntity *entity) const {
        Q_D(const AudioContextInterface);
        return d->tracks.value(entity);
    }
    AudioClipInterface *AudioContextInterface::getAudioClipInterfaceFromEntity(QDspx::AudioClipEntity *entity) const {
        Q_D(const AudioContextInterface);
        return d->clips.value(entity);
    }
}