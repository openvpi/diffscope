#ifndef DIFFSCOPE_GLOBALSTORAGEOBJECT_H
#define DIFFSCOPE_GLOBALSTORAGEOBJECT_H

#include <QObject>
#include <QJSValue>

class GlobalObject;

class GlobalStorageObjectPrivate;

class GlobalStorageObject : public QObject {
    Q_OBJECT
public:
    explicit GlobalStorageObject(GlobalObject *global, const QString &filename);
    ~GlobalStorageObject() override;

    void update();

public slots:
    void setItem(const QString &key, const QString &value);
    QJSValue getItem(const QString &key);
    void removeItem(const QString &key);

private:
    QScopedPointer<GlobalStorageObjectPrivate> d;
};

#endif // DIFFSCOPE_GLOBALSTORAGEOBJECT_H
