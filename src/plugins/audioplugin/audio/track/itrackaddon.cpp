#include "itrackaddon.h"

#include <audioplugin/trackinterface.h>

namespace Audio {
    ITrackAddOn::~ITrackAddOn() = default;

    TrackInterface *ITrackAddOn::trackInterface() const {
        return static_cast<TrackInterface *>(host());
    }
    ITrackAddOn::ITrackAddOn(QObject *parent) : IExecutiveAddOn(parent) {
    }
}