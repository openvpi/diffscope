#include "audiocontextinterface.h"
#include "audiocontextinterface_p.h"

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/internal/projectaddon.h>
#include <audioplugin/formatmanager.h>
#include <audioplugin/formatentry.h>

namespace Audio {

    void AudioContextInterfacePrivate::init(ProjectAddOn *projectAddOn_) {
        Q_Q(AudioContextInterface);
        projectAddOn = projectAddOn_;
        auto model = q->windowHandle()->doc()->dataModel().model();
        QObject::connect(model->tracks(), &QDspx::TrackListEntity::inserted, q, [=](int _, const QVector<QDspx::TrackEntity *> &trackEntities) {
            for (auto trackEntity : trackEntities) {
                handleTrackInserted(trackEntity);
            }
        });
        QObject::connect(model->tracks(), &QDspx::TrackListEntity::aboutToRemove, q, [=](int _, const QVector<QDspx::TrackEntity *> &trackEntities) {
            for (auto trackEntity : trackEntities)
                handleTrackAboutToRemove(trackEntity);
        });

        for (int i = 0; i < model->tracks()->size(); i++) {
            auto trackEntity = model->tracks()->at(i);
            handleTrackInserted(trackEntity);
        }

        // TODO connect doc signals
    }
    void AudioContextInterfacePrivate::handleTrackInserted(QDspx::TrackEntity *trackEntity) {
        Q_Q(AudioContextInterface);
        auto track = trackRegistry.create(trackEntity, q, q);
        tracks.insert(trackEntity, track);
    }
    void AudioContextInterfacePrivate::handleTrackAboutToRemove(QDspx::TrackEntity *trackEntity) {
        auto track = tracks.value(trackEntity);
        if (track) {
            track->quit();
            delete track;
            tracks.remove(trackEntity);
        }
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

    Core::IProjectWindow *AudioContextInterface::windowHandle() const {
        Q_D(const AudioContextInterface);
        return static_cast<Core::IProjectWindow *>(d->projectAddOn->windowHandle());
    }

    AudioContextInterface *AudioContextInterface::get(Core::IProjectWindow *win) {
        return static_cast<AudioContextInterface *>(win->getFirstObject("Audio.AudioContextInterface"));
    }

    OutputSystemInterface *AudioContextInterface::outputSystem() const {
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
    TrackInterface *AudioContextInterface::getTrack(QDspx::TrackEntity *entity) const {
        Q_D(const AudioContextInterface);
        return d->tracks.value(entity);
    }

    talcs::PositionableAudioSource *AudioContextInterface::getFormatSource(const QString &filename, const QVariant &userData, bool isInternal) {
        Q_D(AudioContextInterface);
        auto io = IAudio::instance()->formatManager()->getFormatLoad(filename, userData);
        if (!io)
            return nullptr;
        return d->projectAddOn->postGetFormat(io, filename, isInternal);
    }
}