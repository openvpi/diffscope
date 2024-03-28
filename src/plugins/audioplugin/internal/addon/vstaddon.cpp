#include "vstaddon.h"

namespace Audio {
    VSTAddOn::VSTAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }
    VSTAddOn::~VSTAddOn() {
    }
    void VSTAddOn::initialize() {
    }
    void VSTAddOn::extensionsInitialized() {
    }
    bool VSTAddOn::delayedInitialize() {
        return IWindowAddOn::delayedInitialize();
    }
} // Audio