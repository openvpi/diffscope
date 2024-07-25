#include "projectaddon.h"

#include <TalcsCore/MixerAudioSource.h>
#include <TalcsDspx/DspxProjectContext.h>

#include <coreplugin/iprojectwindow.h>

#include <audioplugin/iaudio.h>
#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/audiocontextinterface_p.h>
#include <audioplugin/internal/audiosystem.h>
#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/vstconnectionsystem.h>

namespace Audio::Internal {

    ProjectAddOn::ProjectAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
        m_audioContextInterface = new AudioContextInterface(this);
        m_projectContext = new talcs::DspxProjectContext(m_audioContextInterface);
        m_projectContext->setFormatManager(IAudio::instance()->formatManager());
        m_projectContext->setTimeConverter({}); // TODO
        m_projectContext->setBufferingReadAheadSize(AudioSystem::outputSystem()->fileBufferingReadAheadSize());
        connect(AudioSystem::outputSystem(), &AbstractOutputSystem::fileBufferingReadAheadSizeChanged, m_projectContext, &talcs::DspxProjectContext::setBufferingReadAheadSize);
    }

    ProjectAddOn::~ProjectAddOn() {
        m_audioContextInterface->outputSystemInterface()->preMixer()->removeSource(m_projectContext->preMixer());
    }

    void ProjectAddOn::initialize() {
        auto iAudio = IAudio::instance();
        iAudio->outputSystemInterface(isVST())->preMixer()->addSource(m_projectContext->preMixer());
        windowHandle()->addObject("Audio.AudioContextInterface", m_audioContextInterface);
        m_audioContextInterface->d_func()->init(this);
        m_audioContextInterface->outputSystemInterface()->preMixer()->addSource(m_projectContext->preMixer());
    }

    void ProjectAddOn::extensionsInitialized() {

    }

    bool ProjectAddOn::delayedInitialize() {
        return false;
    }

    bool ProjectAddOn::isVST() const {
        return static_cast<Core::IProjectWindow *>(windowHandle())->isVST();
    }

    talcs::DspxProjectContext *ProjectAddOn::projectContext() const {
        return m_projectContext;
    }
    AudioContextInterface *ProjectAddOn::audioContextInterface() const {
        return m_audioContextInterface;
    }

    void ProjectAddOn::updateFileCounter(const QString &filename, int count) {
    }

    void ProjectAddOn::setAudioClipToOpenFile(QDspx::AudioClipEntity *entity, const QString &selectedFilter) {
        m_audioClipsToOpenFile->insert(entity, selectedFilter);
    }
    bool ProjectAddOn::checkAudioClipIsToOpenFile(QDspx::AudioClipEntity *entity, QString &selectedFilter) {
        if (m_audioClipsToOpenFile->contains(entity)) {
            selectedFilter = m_audioClipsToOpenFile->value(entity);
            m_audioClipsToOpenFile->remove(entity);
            return true;
        } else {
            return false;
        }
    }

    void ProjectAddOn::addFailedAudioClipToAlert(QDspx::AudioClipEntity *entity) {
        m_failedAudioClipsToAlert.append(entity);
    }
    QList<QDspx::AudioClipEntity *> ProjectAddOn::takeFailedAudioClipsToAlert() {
        auto ret = m_failedAudioClipsToAlert;
        m_failedAudioClipsToAlert.clear();
        return ret;
    }
}