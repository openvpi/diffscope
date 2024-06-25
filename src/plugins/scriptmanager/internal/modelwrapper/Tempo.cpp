#include "Tempo.h"

#include <substate/node.h>

#include <dspxmodel/timelineentity.h>

using QDspx::TempoEntity;

namespace ScriptManager::Internal {

    static const char REF_PROP[] = "c69a0ae3434245e386814883ed583a1c";

    Tempo::Tempo() : Tempo(new TempoEntity) {
    }
    Tempo::Tempo(TempoEntity *entity) : m_entity(entity) {
        int ref = entity->property(REF_PROP).toInt() + 1;
        entity->setProperty(REF_PROP, ref);
        connect(entity, &TempoEntity::positionChanged, this, &Tempo::positionChanged);
        connect(entity, &TempoEntity::valueChanged, this, &Tempo::valueChanged);
    }
    Tempo::~Tempo() {
        int ref = m_entity->property(REF_PROP).toInt() - 1;
        m_entity->setProperty(REF_PROP, ref);
        if (m_entity->internalData()->isFree() && ref == 0)
            delete m_entity;
    }
    int Tempo::position() const {
        return m_entity->position();
    }
    void Tempo::setPosition(int position) const {
        m_entity->setPosition(position);
    }
    double Tempo::value() const {
        return m_entity->value();
    }
    void Tempo::setValue(double value) const {
        m_entity->setValue(value);
    }
}