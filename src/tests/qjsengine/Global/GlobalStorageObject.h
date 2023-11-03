#ifndef DIFFSCOPE_GLOBALSTORAGEOBJECT_H
#define DIFFSCOPE_GLOBALSTORAGEOBJECT_H

#include <QFile>
#include <QJSValue>
#include <QJsonObject>
#include <QObject>

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
    QFile m_storageFile;
    QJsonObject m_jsonStorage;
    bool m_isModified = false;
};

#endif // DIFFSCOPE_GLOBALSTORAGEOBJECT_H
