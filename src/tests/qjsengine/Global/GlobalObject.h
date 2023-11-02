#ifndef DIFFSCOPE_GLOBALOBJECT_H
#define DIFFSCOPE_GLOBALOBJECT_H

#include <QObject>
#include <QJSValue>

class QJSEngine;

class GlobalRegistryObject;
class GlobalStorageObject;
class GlobalObjectPrivate;

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
    QScopedPointer<GlobalObjectPrivate> d;
};

#define jsGlobal GlobalObject::instance()

#endif // DIFFSCOPE_GLOBALOBJECT_H
