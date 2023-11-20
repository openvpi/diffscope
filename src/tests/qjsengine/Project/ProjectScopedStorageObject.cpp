#include "ProjectScopedStorageObject.h"

#include "ProjectObject.h"

ProjectScopedStorageObject::ProjectScopedStorageObject(ProjectObject *project) : QObject(project) {
}

void ProjectScopedStorageObject::setItem(const QString &key, const QJSValue &value) {
    m_itemDict.insert(key, value);
}

QJSValue ProjectScopedStorageObject::getItem(const QString &key) const {
    return m_itemDict.value(key, QJSValue::NullValue);
}

void ProjectScopedStorageObject::removeItem(const QString &key) {
    m_itemDict.remove(key);
}
