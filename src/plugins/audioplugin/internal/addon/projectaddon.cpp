#include "projectaddon.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsCore/PositionableMixerAudioSource.h>
#include <TalcsCore/TransportAudioSource.h>
#include <TalcsCore/Decibels.h>
#include <TalcsFormat/AudioFormatInputSource.h>

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/audiocontextinterface_p.h>
#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/vstconnectionsystem.h>

namespace Audio::Internal {

    class BufferingAudioObjectSource : public QObject, public talcs::BufferingAudioSource {
    public:
        explicit BufferingAudioObjectSource(PositionableAudioSource *src, bool takeOwnership,
                                            int channelCount, qint64 readAheadSize,
                                            bool autoBuffering = true,
                                            QThreadPool *threadPool = nullptr,
                                            QObject *parent = nullptr)
            : QObject(parent), talcs::BufferingAudioSource(src, takeOwnership, channelCount,
                                                           readAheadSize, autoBuffering,
                                                           threadPool) {
        }
        ~BufferingAudioObjectSource() override = default;
    };

    ProjectAddOn::ProjectAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
        m_masterTrackMixer = new talcs::PositionableMixerAudioSource();
        m_masterTrackControlMixer = new talcs::PositionableMixerAudioSource;
        m_masterTrackControlMixer->addSource(m_masterTrackMixer, true);
        m_postMixer = new talcs::PositionableMixerAudioSource;
        m_postMixer->addSource(m_masterTrackControlMixer, true);
        m_tpSrc = new talcs::TransportAudioSource(m_postMixer, true);
        m_preMixer = std::make_unique<talcs::MixerAudioSource>();
        m_preMixer->addSource(m_tpSrc, true);

        m_audioContextInterface = new AudioContextInterface(this);
    }

    ProjectAddOn::~ProjectAddOn() {
        auto iAudio = IAudio::instance();
        iAudio->outputSystemInterface(isVST())->preMixer()->removeSource(m_preMixer.get());
        m_tpSrc->setSource(nullptr);
    }

    void ProjectAddOn::initialize() {
        auto iAudio = IAudio::instance();
        iAudio->outputSystemInterface(isVST())->preMixer()->addSource(m_preMixer.get());
        windowHandle()->addObject("Audio.AudioContextInterface", m_audioContextInterface);

        auto model = static_cast<Core::IProjectWindow *>(windowHandle())->doc()->dataModel().model();

        connect(model->master()->control(), &QDspx::BusControlEntity::gainChanged, this, [=](double gainDecibel) {
            handleEntityGainChange(gainDecibel);
        });
        connect(model->master()->control(), &QDspx::BusControlEntity::panChanged, this, [=](double pan) {
            handleEntityPanChange(pan);
        });
        connect(model->master()->control(), &QDspx::BusControlEntity::muteChanged, this, [=](bool isMuted) {
            handleEntityMuteChange(isMuted);
        });

        handleEntityGainChange(model->master()->control()->gain());
        handleEntityPanChange(model->master()->control()->pan());
        handleEntityMuteChange(model->master()->control()->mute());

        m_audioContextInterface->d_func()->init(this);
    }

    void ProjectAddOn::extensionsInitialized() {

    }

    bool ProjectAddOn::delayedInitialize() {
        return false;
    }

    bool ProjectAddOn::isVST() const {
        return static_cast<Core::IProjectWindow *>(windowHandle())->isVST();
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
        return m_masterTrackMixer;
    }

    talcs::PositionableAudioSource *ProjectAddOn::postGetFormat(talcs::AbstractAudioFormatIO *io, const QString &filename, bool isInternal) {
        auto fmtSrc = new talcs::AudioFormatInputSource(io, true);
        fmtSrc->setStereoize(true);
        AbstractOutputSystem *outputSys = isVST() ? static_cast<AbstractOutputSystem *>(AudioSystem::vstConnectionSystem()) : static_cast<AbstractOutputSystem *>(AudioSystem::outputSystem());
        auto readAheadSize = outputSys->fileBufferingReadAheadSize();
        auto bufSrc = new BufferingAudioObjectSource(fmtSrc, true, 2, readAheadSize, true, nullptr, this);
        connect(outputSys, &AbstractOutputSystem::fileBufferingReadAheadSizeChanged, bufSrc, [=](qint64 size) {
            bufSrc->setReadAheadSize(size);
        });
        connect(bufSrc, &QObject::destroyed, this, [=] {
            files.remove(bufSrc);
        });
        files.append(bufSrc, {filename, bufSrc, io, isInternal});
        return bufSrc;
    }

    void ProjectAddOn::handleEntityGainChange(double gainDecibel) const {
        m_masterTrackControlMixer->setGain(talcs::Decibels::decibelsToGain(static_cast<float>(gainDecibel)));
    }
    void ProjectAddOn::handleEntityPanChange(double pan) const {
        m_masterTrackControlMixer->setPan(static_cast<float>(pan));
    }
    void ProjectAddOn::handleEntityMuteChange(bool isMute) const {
        m_masterTrackControlMixer->setSilentFlags(isMute ? -1 : 0);
    }

}