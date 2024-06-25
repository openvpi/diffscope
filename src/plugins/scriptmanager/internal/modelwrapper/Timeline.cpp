#include "Timeline.h"

#include <QQmlEngine>

#include <dspxmodel/timelineentity.h>

#include <scriptmanager/internal/TempoList.h>
#include <scriptmanager/internal/TimelineLabelList.h>

using QDspx::TimelineEntity;

namespace ScriptManager::Internal {

    Timeline::Timeline(QDspx::TimelineEntity *entity) : m_entity(entity) {
    }
    Timeline::~Timeline() = default;

    QJSValue Timeline::timeSignatures() const {
        return {}; // TODO
    }
    QJSValue Timeline::tempos() const {
        if (!m_wrappedTempos.isUndefined())
            return m_wrappedTempos;
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        return m_wrappedTempos = (new TempoList(m_entity->tempos()))->wrap(engine);
    }
    QJSValue Timeline::labels() const {
        if (!m_wrappedLabels.isUndefined())
            return m_wrappedLabels;
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        return m_wrappedLabels = (new TimelineLabelList(m_entity->labels()))->wrap(engine);
    }
}