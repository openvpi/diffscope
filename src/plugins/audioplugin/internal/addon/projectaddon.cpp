#include "projectaddon.h"

namespace Audio {
    ProjectAddOn::ProjectAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }
    ProjectAddOn::~ProjectAddOn() {
    }
    void ProjectAddOn::initialize() {
    }
    void ProjectAddOn::extensionsInitialized() {
    }
    bool ProjectAddOn::delayedInitialize() {
        return IWindowAddOn::delayedInitialize();
    }
} // Audio