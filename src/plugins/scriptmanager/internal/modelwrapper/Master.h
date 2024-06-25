#ifndef SCRIPT_MANAGER_MASTER_H
#define SCRIPT_MANAGER_MASTER_H

#include <QObject>
#include <QJSValue>

namespace QDspx {
    class MasterEntity;
}

namespace ScriptManager::Internal {

    class Master : public QObject {
        Q_OBJECT
        Q_PROPERTY(QJSValue control READ control)
    public:
        explicit Master(QDspx::MasterEntity *entity);
        ~Master() override;

    public:
        QJSValue control() const;

    private:
        QDspx::MasterEntity *m_entity;
    };

}

#endif // SCRIPT_MANAGER_MASTER_H
