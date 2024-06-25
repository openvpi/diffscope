#include "TimelineLabel.h"

#include <substate/node.h>

#include <dspxmodel/timelineentity.h>

using QDspx::TimelineLabelEntity;

namespace ScriptManager::Internal {

    static const char REF_PROP[] = "73e56ce030ed4190bffbc61c132c16d0";

    TimelineLabel::TimelineLabel() : TimelineLabel(new TimelineLabelEntity) {
    }
    TimelineLabel::TimelineLabel(TimelineLabelEntity *entity) : m_entity(entity) {
        int ref = entity->property(REF_PROP).toInt() + 1;
        entity->setProperty(REF_PROP, ref);
        connect(entity, &TimelineLabelEntity::positionChanged, this, &TimelineLabel::positionChanged);
        connect(entity, &TimelineLabelEntity::textChanged, this, &TimelineLabel::textChanged);
    }
    TimelineLabel::~TimelineLabel() {
        int ref = m_entity->property(REF_PROP).toInt() - 1;
        m_entity->setProperty(REF_PROP, ref);
        if (m_entity->internalData()->isFree() && ref == 0)
            delete m_entity;
    }
    int TimelineLabel::position() const {
        return m_entity->position();
    }
    void TimelineLabel::setPosition(int position) {
        m_entity->setPosition(position);
    }
    QString TimelineLabel::text() const {
        return m_entity->text();
    }
    void TimelineLabel::setText(const QString &text) {
        m_entity->setText(text);
    }
}