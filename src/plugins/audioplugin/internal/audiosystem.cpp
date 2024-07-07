#include "audiosystem.h"

#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/internal/vstconnectionsystem.h>
#include <audioplugin/internal/midisystem.h>

namespace Audio::Internal {

    static AudioSystem *m_instance = nullptr;

    AudioSystem::AudioSystem(QObject *parent) : QObject(parent) {
        m_instance = this;
        m_outputSystem = new OutputSystem(this);
        m_vstConnectionSystem = new VSTConnectionSystem(this);
        m_midiSystem = new MidiSystem(this);
    }
    AudioSystem::~AudioSystem() {
        m_instance = nullptr;
    }
    AudioSystem *AudioSystem::instance() {
        return m_instance;
    }
    OutputSystem *AudioSystem::outputSystem() {
        return m_instance->m_outputSystem;
    }
    VSTConnectionSystem *AudioSystem::vstConnectionSystem() {
        return m_instance->m_vstConnectionSystem;
    }
    MidiSystem *AudioSystem::midiSystem() {
        return m_instance->m_midiSystem;
    }
}