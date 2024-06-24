#include "TrackList.h"

#include <QJSEngine>

#include <dspxmodel/trackentity.h>

namespace ScriptManager::Internal {

    using namespace QDspx;

    TrackList::TrackList(QDspx::TrackListEntity *entity) : m_entity(entity) {
    }
    TrackList::~TrackList() = default;
    void TrackList::insert(int index, const QJSValue &items) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        QVector<TrackEntity *> trackEntities;
        for (int i = 0, length = items.property("length").toInt(); i < length; i++) {
            auto trackEntity = qobject_cast<TrackEntity *>(items.property(i).toQObject());
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
        return {};
    }
    int TrackList::size() const {
        return m_entity->size();
    }
}