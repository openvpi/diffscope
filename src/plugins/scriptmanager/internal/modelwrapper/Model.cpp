#include "Model.h"

#include <QQmlEngine>

#include <dspxmodel/modelentity.h>

#include <scriptmanager/internal/Master.h>
#include <scriptmanager/internal/Timeline.h>
#include <scriptmanager/internal/TrackList.h>

using QDspx::ModelEntity;

namespace ScriptManager::Internal {
    Model::Model(ModelEntity *entity) : m_entity(entity) {
    }
    Model::~Model() = default;

    QJSValue Model::global() const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        auto o = engine->newQObject(m_entity->global());
        QQmlEngine::setObjectOwnership(m_entity->global(), QQmlEngine::CppOwnership);
        return o;
    }
    QObject *Model::master() const {
        return new Master(m_entity->master());
    }
    QObject *Model::timeline() const {
        return new Timeline(m_entity->timeline());
    }
    QJSValue Model::tracks() const {
        if (!m_wrappedTrackListEntity.isUndefined())
            return m_wrappedTrackListEntity;
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        return m_wrappedTrackListEntity = (new TrackList(m_entity->tracks()))->wrap(engine);
    }
} // ScriptManager