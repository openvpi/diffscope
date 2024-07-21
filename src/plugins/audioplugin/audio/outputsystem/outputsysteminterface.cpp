#include "outputsysteminterface.h"
#include "outputsysteminterface_p.h"

#include <audioplugin/internal/outputsystem.h>
#include <audioplugin/iaudio.h>

#include <TalcsDevice/OutputContext.h>

namespace Audio {

    using namespace Internal;

    void OutputSystemInterfacePrivate::init(AbstractOutputSystem *abstractOutputSystem_, bool isVST_) {
        Q_Q(OutputSystemInterface);
        abstractOutputSystem = abstractOutputSystem_;
        isVST = isVST_;
        QObject::connect(abstractOutputSystem->context(), &talcs::AbstractOutputContext::bufferSizeChanged, q, &OutputSystemInterface::bufferSizeChanged);
        QObject::connect(abstractOutputSystem->context(), &talcs::AbstractOutputContext::sampleRateChanged, q, &OutputSystemInterface::sampleRateChanged);
        QObject::connect(abstractOutputSystem->context(), &talcs::AbstractOutputContext::deviceChanged, q, &OutputSystemInterface::deviceChanged);
    }

    OutputSystemInterface::OutputSystemInterface(QObject *parent) : QObject(parent), d_ptr(new OutputSystemInterfacePrivate) {
        Q_D(OutputSystemInterface);
        d->q_ptr = this;
    }
    OutputSystemInterface::~OutputSystemInterface() {
    }
    bool OutputSystemInterface::isVST() const {
        Q_D(const OutputSystemInterface);
        return d->isVST;
    }
    talcs::AudioDriverManager *OutputSystemInterface::audioDriverManager() const {
        Q_D(const OutputSystemInterface);
        if (!d->isVST)
            return static_cast<OutputSystem *>(d->abstractOutputSystem)->outputContext()->driverManager();
        return nullptr;
    }
    talcs::AudioDriver *OutputSystemInterface::audioDriver() const {
        Q_D(const OutputSystemInterface);
        if (!d->isVST)
            return static_cast<OutputSystem *>(d->abstractOutputSystem)->outputContext()->driver();
        return nullptr;
    }
    talcs::AudioDevice *OutputSystemInterface::audioDevice() const {
        Q_D(const OutputSystemInterface);
        return d->abstractOutputSystem->context()->device();
    }
    talcs::MixerAudioSource *OutputSystemInterface::preMixer() const {
        Q_D(const OutputSystemInterface);
        return d->abstractOutputSystem->context()->preMixer();
    }
    bool OutputSystemInterface::isReady() {
        Q_D(OutputSystemInterface);
        return d->abstractOutputSystem->isReady();
    }
}