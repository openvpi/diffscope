#ifndef SCRIPT_MANAGER_VECTORENTITYWRAPPER_H
#define SCRIPT_MANAGER_VECTORENTITYWRAPPER_H

#include <QObject>
#include <QJSValue>

namespace ScriptManager::Internal {

    class VectorEntityWrapper : public QObject {
        Q_OBJECT
    public:
        explicit VectorEntityWrapper();
        ~VectorEntityWrapper() override;

        QJSValue wrap(QJSEngine *engine);

    public slots:
        virtual void insert(int index, const QJSValue &items) = 0;
        virtual void move(int index, int count, int dest) = 0;
        virtual void remove(int index, int count) = 0;
        virtual QJSValue at(int index) const = 0;
        virtual int size() const = 0;

    signals:
        void inserted(int index, const QJSValue &items);
        void aboutToMove(int index, int count, int dest);
        void moved(int index, int count, int dest);
        void aboutToRemove(int index, const QJSValue &items);
        void removed(int index, int count);
    };

}

#endif // SCRIPT_MANAGER_VECTORENTITYWRAPPER_H
