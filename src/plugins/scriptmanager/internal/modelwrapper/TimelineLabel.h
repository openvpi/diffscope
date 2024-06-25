#ifndef SCRIPT_MANAGER_TIMELINELABEL_H
#define SCRIPT_MANAGER_TIMELINELABEL_H

#include <QObject>

namespace QDspx {
    class TimelineLabelEntity;
}

namespace ScriptManager::Internal {

    class TimelineLabelList;

    class TimelineLabel : public QObject {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition)
        Q_PROPERTY(QString text READ text WRITE setText)
        friend class TimelineLabelList;
    public:
        Q_INVOKABLE explicit TimelineLabel();
        explicit TimelineLabel(QDspx::TimelineLabelEntity *entity);
        ~TimelineLabel() override;

        int position() const;
        void setPosition(int position);

        QString text() const;
        void setText(const QString &text);

    signals:
        void positionChanged(int position);
        void textChanged(const QString &text);

    private:
        QDspx::TimelineLabelEntity *m_entity;
    };

} // ScriptManager

#endif // SCRIPT_MANAGER_TIMELINELABEL_H
