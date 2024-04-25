#include "iaudio.h"
#include "iaudio_p.h"

namespace Audio {
    static IAudio *m_instance = nullptr;
    IAudio::IAudio(QObject *parent) : QObject(parent), d_ptr(new IAudioPrivate) {
        m_instance = this;
    }
    IAudio::~IAudio() {
        m_instance = nullptr;
    }
    IAudio *IAudio::instance() {
        return m_instance;
    }
    OutputSystemInterface *IAudio::outputSystemInterface() const {
        Q_D(const IAudio);
        return d->outputSystemInterface;
    }
    OutputSystemInterface *IAudio::vstOutputSystemInterface() const {
        Q_D(const IAudio);
        return d->vstOutputSystemInterface;
    }
    void IAudio::installOutputSystemAddOn(const QMetaObject *clazz) {
        Q_D(IAudio);
        d->outputSystemAddOnClasses.append(clazz);
    }
    QList<const QMetaObject *> IAudio::outputSystemAddOns() const {
        Q_D(const IAudio);
        return d->outputSystemAddOnClasses;
    }
    void IAudio::installAudioContextAddOn(const QMetaObject *clazz) {
        Q_D(IAudio);
        d->audioContextAddOnClasses.append(clazz);
    }
    QList<const QMetaObject *> IAudio::audioContextAddOns() const {
        Q_D(const IAudio);
        return d->audioContextAddOnClasses;
    }
}