#ifndef SCRIPT_MANAGER_TEMPO_H
#define SCRIPT_MANAGER_TEMPO_H

#include <QObject>

namespace QDspx {
    class TempoEntity;
}

namespace ScriptManager::Internal {

    class TempoList;

    class Tempo : public QObject {
        Q_OBJECT
        Q_PROPERTY(int position READ position WRITE setPosition)
        Q_PROPERTY(double value READ value WRITE setValue)
        friend class TempoList;
    public:
        Q_INVOKABLE explicit Tempo();
        explicit Tempo(QDspx::TempoEntity *entity);
        ~Tempo() override;

        int position() const;
        void setPosition(int position) const;

        double value() const;
        void setValue(double value) const;

    signals:
        void positionChanged(int position);
        void valueChanged(double value);

    private:
        QDspx::TempoEntity *m_entity;
    };

}

#endif // SCRIPT_MANAGER_TEMPO_H
