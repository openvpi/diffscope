#include "Master.h"

#include <QQmlEngine>

#include <dspxmodel/modelentity.h>

using QDspx::MasterEntity;

namespace ScriptManager::Internal {
    Master::Master(MasterEntity *entity) : m_entity(entity) {
    }
    Master::~Master() = default;

    QJSValue Master::control() const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        auto o = engine->newQObject(m_entity->control());
        QQmlEngine::setObjectOwnership(m_entity->control(), QQmlEngine::CppOwnership);
        return o;
    }
}