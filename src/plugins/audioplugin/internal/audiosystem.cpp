#include "audiosystem.h"

#include "outputsystem.h"
#include "vstconnectionsystem.h"

namespace Audio {

    static AudioSystem *m_instance = nullptr;

    AudioSystem::AudioSystem(QObject *parent) : QObject(parent) {
        m_instance = this;
        m_outputSystem = new OutputSystem(this);
        m_vstConnectionSystem = new VSTConnectionSystem(this);
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
} // Audio