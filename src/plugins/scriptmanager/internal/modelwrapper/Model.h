#ifndef SCRIPT_MANAGER_MODEL_H
#define SCRIPT_MANAGER_MODEL_H

#include <QJSValue>
#include <QObject>

namespace QDspx {
    class ModelEntity;
}

namespace ScriptManager::Internal {

    class Model : public QObject {
        Q_OBJECT
        Q_PROPERTY(QObject* global READ global)
        Q_PROPERTY(QObject* master READ master)
        Q_PROPERTY(QObject* timeline READ timeline)
        Q_PROPERTY(QJSValue tracks READ tracks)
    public:
        explicit Model(QDspx::ModelEntity *entity);
        ~Model() override;

        QObject *global() const;
        QObject *master() const;
        QObject *timeline() const;
        QJSValue tracks() const;

    private:
        QDspx::ModelEntity *m_entity;
        mutable QJSValue m_wrappedTrackListEntity;
    };

}

#endif // SCRIPT_MANAGER_MODEL_H
