#include "ioutputsystemaddon.h"
#include "ioutputsystemaddon_p.h"

#include <audioplugin/outputsysteminterface.h>
#include <audioplugin/private/outputsysteminterface_p.h>
#include <audioplugin/internal/abstractoutputsystem.h>

namespace Audio {
    IOutputSystemAddOn::IOutputSystemAddOn(QObject *parent) : QObject(parent), d_ptr(new IOutputSystemAddOnPrivate) {
        Q_D(IOutputSystemAddOn);
        d->q_ptr = this;
    }
    IOutputSystemAddOn::~IOutputSystemAddOn() {

    }
    bool IOutputSystemAddOn::delayedInitialize() {
        return false;
    }
    void IOutputSystemAddOn::handleBufferSizeChange() {
    }
    void IOutputSystemAddOn::handleSampleRateChange() {
    }
    void IOutputSystemAddOn::handleDeviceChange() {
    }
    bool IOutputSystemAddOn::substituteSource(talcs::AudioSource *source) {
        Q_D(IOutputSystemAddOn);
        if (d->outputSystemInterface->isSourceSubstituted())
            return false;
        d->outputSystemInterface->d_func()->abstractOutputSystem->setSubstitutedSource(source, this);
        return true;
    }
    bool IOutputSystemAddOn::resetSource() {
        Q_D(IOutputSystemAddOn);
        if (d->outputSystemInterface->d_func()->abstractOutputSystem->sourceSubstitutor() != this)
            return false;
        d->outputSystemInterface->d_func()->abstractOutputSystem->resetSubstitutedSource();
        return true;
    }
    void IOutputSystemAddOn::handleSubstitutedSourceAfterControl(double gain, double pan) {
    }
    OutputSystemInterface *IOutputSystemAddOn::outputSystemInterface() const {
        Q_D(const IOutputSystemAddOn);
        return d->outputSystemInterface;
    }
} // Audio