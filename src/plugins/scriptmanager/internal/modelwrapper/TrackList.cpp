#include "TrackList.h"

#include <QJSEngine>

#include <dspxmodel/trackentity.h>

using QDspx::TrackListEntity;
using QDspx::TrackEntity;

namespace ScriptManager::Internal {

    TrackList::TrackList(TrackListEntity *entity) : m_entity(entity) {
        connect(entity, &TrackListEntity::inserted, this, [=](int index, const QVector<QDspx::TrackEntity *> &items) {
            auto engine = qjsEngine(this);
            Q_ASSERT(engine);
            // TODO
        });
        connect(entity, &TrackListEntity::aboutToMove, this, &TrackList::aboutToMove);
        connect(entity, &TrackListEntity::moved, this, &TrackList::moved);
        connect(entity, &TrackListEntity::aboutToRemove, this, [=](int index, const QVector<QDspx::TrackEntity *> &items) {
            auto engine = qjsEngine(this);
            Q_ASSERT(engine);
            // TODO
        });
        connect(entity, &TrackListEntity::removed, this, &TrackList::removed);
    }
    TrackList::~TrackList() = default;

    void TrackList::insert(int index, const QJSValue &items) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        QVector<TrackEntity *> trackEntities;
        for (int i = 0, length = items.property("length").toInt(); i < length; i++) {
            auto trackEntity = qobject_cast<TrackEntity *>(items.property(i).toQObject()); // TODO
            if (!trackEntity) {
                engine->throwError(QJSValue::TypeError, "Invalid argument");
                return;
            }
            trackEntities.append(trackEntity);
            if (!m_entity->insert(index, trackEntities)) {
                engine->throwError(QJSValue::TypeError, "Invalid argument");
            }
        }
    }
    void TrackList::move(int index, int count, int dest) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!m_entity->move(index, count, dest)) {
            engine->throwError(QJSValue::TypeError, "Invalid argument");
        }
    }
    void TrackList::remove(int index, int count) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!m_entity->remove(index, count)) {
            engine->throwError(QJSValue::TypeError, "Invalid argument");
        }
    }
    QJSValue TrackList::at(int index) const {
        return {}; // TODO
    }
    int TrackList::size() const {
        return m_entity->size();
    }
}