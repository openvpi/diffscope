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
    OutputSystemInterface *IAudio::outputSystemInterface(bool isVST) const {
        Q_D(const IAudio);
        return isVST ? d->vstOutputSystemInterface : d->outputSystemInterface;
    }
    FormatManager *IAudio::formatManager() const {
        Q_D(const IAudio);
        return d->formatManager;
    }
}