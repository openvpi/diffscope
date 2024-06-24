#include "Model.h"

#include <QJSEngine>

#include <dspxmodel/modelentity.h>

#include <scriptmanager/internal/TrackList.h>

namespace ScriptManager::Internal {
    Model::Model(QDspx::ModelEntity *entity) : m_entity(entity) {
    }
    Model::~Model() = default;
    QObject *Model::global() const {
        return nullptr;
    }
    QObject *Model::master() const {
        return nullptr;
    }
    QObject *Model::timeline() const {
        return nullptr;
    }
    QJSValue Model::tracks() const {
        if (!m_wrappedTrackListEntity.isUndefined())
            return m_wrappedTrackListEntity;
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        return m_wrappedTrackListEntity = (new TrackList(m_entity->tracks()))->wrap(engine);
    }
} // ScriptManager