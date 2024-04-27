#include "iaudioclipaddon.h"

#include <audioplugin/audioclipinterface.h>

namespace Audio {
    IAudioClipAddOn::IAudioClipAddOn(QObject *parent) : Core::IExecutiveAddOn(parent) {
    }

    IAudioClipAddOn::~IAudioClipAddOn() = default;

    AudioClipInterface *IAudioClipAddOn::audioClipInterface() const {
        return static_cast<AudioClipInterface *>(host());
    }
}