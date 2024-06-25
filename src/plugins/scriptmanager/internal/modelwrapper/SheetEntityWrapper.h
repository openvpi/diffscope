#ifndef SCRIPT_MANAGER_SHEETENTITYWRAPPER_H
#define SCRIPT_MANAGER_SHEETENTITYWRAPPER_H

#include <QObject>
#include <QJSValue>

namespace ScriptManager::Internal {

    class SheetEntityWrapper : public QObject {
        Q_OBJECT
    public:
        explicit SheetEntityWrapper();
        ~SheetEntityWrapper() override;

        QJSValue wrap(QJSEngine *engine);

    public slots:
        virtual void insert(const QJSValue &item) = 0;
        virtual void remove(const QJSValue &idOrItem) = 0;
        virtual QJSValue value(int id) const = 0;
        virtual int indexOf(const QJSValue &item) = 0;
        virtual QList<int> ids() const = 0;
        virtual int size() const = 0;

    signals:
        void inserted(int id, const QJSValue &item);
        void aboutToRemove(int id, const QJSValue &item);
        void removed(int id, const QJSValue &item);
    };

} // ScriptManager

#define SHEET_ENTITY_WRAPPER_DECLARATION(Class, EntityClass)                                       \
    public:                                                                                        \
       explicit Class(EntityClass *entity);                                                        \
       ~Class() override;                                                                          \
                                                                                                   \
    public slots:                                                                                  \
        void insert(const QJSValue &item) override;                                                \
        void remove(const QJSValue &idOrItem) override;                                            \
        QJSValue value(int id) const override;                                                     \
        int indexOf(const QJSValue &item) override;                                                \
        QList<int> ids() const override;                                                           \
        int size() const override;                                                                 \
                                                                                                   \
    private:                                                                                       \
        EntityClass *m_entity;

#define SHEET_ENTITY_WRAPPER_IMPLEMENTATION_QOBJECT(Class, EntityClass, ItemEntityClass)                  \
    Class::Class(EntityClass *entity) : m_entity(entity) {                                                \
    }                                                                                                     \
    Class::~Class() = default;                                                                            \
    void Class::insert(const QJSValue &item) {                                                            \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        if (auto itemEntity = qobject_cast<ItemEntityClass *>(item.toQObject()); itemEntity) {            \
            m_entity->insert(itemEntity);                                                                 \
        } else {                                                                                          \
            engine->throwError(QJSValue::TypeError, "Invalid argument");                                  \
            return;                                                                                       \
        }                                                                                                 \
    }                                                                                                     \
    void Class::remove(const QJSValue &idOrItem) {                                                        \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        if (idOrItem.isNumber()) {                                                                        \
            m_entity->remove(idOrItem.toInt());                                                           \
            return;                                                                                       \
        } else if (auto itemEntity = qobject_cast<ItemEntityClass *>(idOrItem.toQObject()); itemEntity) { \
            m_entity->remove(itemEntity);                                                                 \
        } else {                                                                                          \
            engine->throwError(QJSValue::TypeError, "Invalid argument");                                  \
            return;                                                                                       \
        }                                                                                                 \
    }                                                                                                     \
    QJSValue Class::value(int id) const {                                                                 \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        auto itemEntity = m_entity->value(id);                                                            \
        auto o = engine->newQObject(itemEntity);                                                          \
        QQmlEngine::setObjectOwnership(itemEntity, QQmlEngine::CppOwnership);                             \
        return o;                                                                                         \
    }                                                                                                     \
    int Class::indexOf(const QJSValue &item) {                                                            \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        if (auto itemEntity = qobject_cast<ItemEntityClass *>(item.toQObject()); itemEntity) {            \
            return m_entity->indexOf(itemEntity);                                                         \
        } else {                                                                                          \
            engine->throwError(QJSValue::TypeError, "Invalid argument");                                  \
            return 0;                                                                                     \
        }                                                                                                 \
    }                                                                                                     \
    QList<int> Class::ids() const {                                                                       \
        return m_entity->ids();                                                                           \
    }                                                                                                     \
    int Class::size() const {                                                                             \
        return m_entity->size();                                                                          \
    }

#define SHEET_ENTITY_WRAPPER_IMPLEMENTATION_WRAPPED(Class, EntityClass, ItemClass)                  \
    Class::Class(EntityClass *entity) : m_entity(entity) {                                                \
    }                                                                                                     \
    Class::~Class() = default;                                                                            \
    void Class::insert(const QJSValue &item) {                                                            \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        if (auto itemObject = qobject_cast<ItemClass *>(item.toQObject()); itemObject) {            \
            m_entity->insert(itemObject->m_entity);                                                                 \
        } else {                                                                                          \
            engine->throwError(QJSValue::TypeError, "Invalid argument");                                  \
            return;                                                                                       \
        }                                                                                                 \
    }                                                                                                     \
    void Class::remove(const QJSValue &idOrItem) {                                                        \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        if (idOrItem.isNumber()) {                                                                        \
            m_entity->remove(idOrItem.toInt());                                                           \
            return;                                                                                       \
        } else if (auto itemObject = qobject_cast<ItemClass *>(idOrItem.toQObject()); itemObject) { \
            m_entity->remove(itemObject->m_entity);                                                                 \
        } else {                                                                                          \
            engine->throwError(QJSValue::TypeError, "Invalid argument");                                  \
            return;                                                                                       \
        }                                                                                                 \
    }                                                                                                     \
    QJSValue Class::value(int id) const {                                                                 \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        auto itemEntity = m_entity->value(id);                                                            \
        auto o = engine->newQObject(itemEntity);                                                          \
        QQmlEngine::setObjectOwnership(itemEntity, QQmlEngine::CppOwnership);                             \
        return o;                                                                                         \
    }                                                                                                     \
    int Class::indexOf(const QJSValue &item) {                                                            \
        auto engine = qjsEngine(this);                                                                    \
        Q_ASSERT(engine);                                                                                 \
        if (auto itemObject = qobject_cast<ItemClass *>(item.toQObject()); itemObject) {            \
            return m_entity->indexOf(itemObject->m_entity);                                                         \
        } else {                                                                                          \
            engine->throwError(QJSValue::TypeError, "Invalid argument");                                  \
            return 0;                                                                                     \
        }                                                                                                 \
    }                                                                                                     \
    QList<int> Class::ids() const {                                                                       \
        return m_entity->ids();                                                                           \
    }                                                                                                     \
    int Class::size() const {                                                                             \
        return m_entity->size();                                                                          \
    }


#endif // SCRIPT_MANAGER_SHEETENTITYWRAPPER_H
