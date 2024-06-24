#ifndef SCRIPT_MANAGER_TRACKLIST_H
#define SCRIPT_MANAGER_TRACKLIST_H

#include <scriptmanager/internal/VectorEntityWrapper.h>

namespace QDspx {
    class TrackListEntity;
}

namespace ScriptManager::Internal {

    class TrackList : public VectorEntityWrapper {
        Q_OBJECT
    public:
        explicit TrackList(QDspx::TrackListEntity *entity);
        ~TrackList() override;

    public slots:
        void insert(int index, const QJSValue &items) override;
        void move(int index, int count, int dest) override;
        void remove(int index, int count) override;
        QJSValue at(int index) const override;
        int size() const override;

    private:
        QDspx::TrackListEntity *m_entity;
    };

}

#endif // SCRIPT_MANAGER_TRACKLIST_H
