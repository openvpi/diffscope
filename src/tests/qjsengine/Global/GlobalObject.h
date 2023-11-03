#ifndef DIFFSCOPE_GLOBALOBJECT_H
#define DIFFSCOPE_GLOBALOBJECT_H

#include <QJSValue>
#include <QObject>

class QJSEngine;

class GlobalRegistryObject;
class GlobalStorageObject;

class GlobalObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue registry READ jsRegistry)
    Q_PROPERTY(QJSValue storage READ jsStorage)
public:
    explicit GlobalObject(QObject *parent = nullptr);
    ~GlobalObject() override;
    static GlobalObject *instance();

    QJSEngine *engine() const;
    QJSValue load(const QString &scriptFilename);
    GlobalRegistryObject *registry() const;
    GlobalStorageObject *storage() const;

    QJSValue jsRegistry() const;
    QJSValue jsStorage() const;

private:
    QJSEngine *m_engine;

    GlobalRegistryObject *m_registry;
    GlobalStorageObject *m_storage;

    QJSValue m_registryObject;
    QJSValue m_storageObject;
};

#define jsGlobal GlobalObject::instance()
#define JS_QOBJ jsGlobal->engine()->newQObject
#define JS_THROW jsGlobal->engine()->throwError

#endif // DIFFSCOPE_GLOBALOBJECT_H
