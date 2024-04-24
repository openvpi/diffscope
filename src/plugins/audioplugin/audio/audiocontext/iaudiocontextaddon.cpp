#include "iaudiocontextaddon.h"
#include "iaudiocontextaddon_p.h"

namespace Audio {
    IAudioContextAddOn::IAudioContextAddOn(QObject *parent) : QObject(parent), d_ptr(new IAudioContextAddOnPrivate) {
        Q_D(IAudioContextAddOn);
        d->q_ptr = this;
    }

    IAudioContextAddOn::~IAudioContextAddOn() = default;

    bool IAudioContextAddOn::delayedInitialize() {
        return false;
    }
} // Audio