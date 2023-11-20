#ifndef DIFFSCOPE_PROJECTSCOPEDSTORAGEOBJECT_H
#define DIFFSCOPE_PROJECTSCOPEDSTORAGEOBJECT_H

#include <QObject>
#include <QMap>
#include <QJSValue>

class ProjectObject;

class ProjectScopedStorageObject : public QObject {
    Q_OBJECT
public:
    explicit ProjectScopedStorageObject(ProjectObject *project);

public slots:
    void setItem(const QString &key, const QJSValue &value);
    QJSValue getItem(const QString &key) const;
    void removeItem(const QString &key);

private:
    QMap<QString, QJSValue> m_itemDict;
};

#endif // DIFFSCOPE_PROJECTSCOPEDSTORAGEOBJECT_H
