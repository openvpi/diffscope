#include "devicetesteraddon.h"

#include <TalcsCore/MixerAudioSource.h>

#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/internal/audiodevicetesteraudiosource.h>

namespace Audio {
    static DeviceTesterAddOn *m_instance[2] = {nullptr, nullptr};
    DeviceTesterAddOn::DeviceTesterAddOn(QObject *parent) : IOutputSystemAddOn(parent) {
        m_src = nullptr;
    }
    DeviceTesterAddOn::~DeviceTesterAddOn() {
        if (m_instance[0] == this) {
            m_instance[0] = nullptr;
        } else if (m_instance[1] == this) {
            m_instance[1] = nullptr;
        }
    }
    void DeviceTesterAddOn::testDevice(bool isVST) {
        if (isVST) {
            m_instance[1]->m_src->playTestSound();
        } else {
            m_instance[0]->m_src->playTestSound();
        }
    }
    void DeviceTesterAddOn::initialize() {
        if (outputSystemInterface()->isVST()) {
            m_instance[1] = this;
        } else {
            m_instance[0] = this;
        }
        outputSystemInterface()->preMixer()->addSource(m_src = new AudioDeviceTesterAudioSource, true);
    }
    void DeviceTesterAddOn::extensionInitialized() {
    }
}