#ifndef SCRIPT_MANAGER_TIMELINE_H
#define SCRIPT_MANAGER_TIMELINE_H

#include <QObject>
#include <QJSValue>

namespace QDspx {
    class TimelineEntity;
}

namespace ScriptManager::Internal {

    class Timeline : public QObject {
        Q_OBJECT
        Q_PROPERTY(QJSValue timeSignatures READ timeSignatures)
        Q_PROPERTY(QJSValue tempos READ tempos)
        Q_PROPERTY(QJSValue labels READ labels)
    public:
        explicit Timeline(QDspx::TimelineEntity *entity);
        ~Timeline() override;

        QJSValue timeSignatures() const;
        QJSValue tempos() const;
        QJSValue labels() const;

    private:
        QDspx::TimelineEntity *m_entity;
        mutable QJSValue m_wrappedTimeSignatures;
        mutable QJSValue m_wrappedTempos;
        mutable QJSValue m_wrappedLabels;
    };

}

#endif // SCRIPT_MANAGER_TIMELINE_H
