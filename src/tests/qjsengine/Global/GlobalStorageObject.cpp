#include "GlobalStorageObject.h"

#include "GlobalObject.h"

#include <QJsonDocument>

GlobalStorageObject::GlobalStorageObject(GlobalObject *global, const QString &filename)
    : QObject(global), m_storageFile(filename) {
    m_storageFile.open(QFile::ReadWrite);
    m_jsonStorage = QJsonDocument::fromJson(m_storageFile.readAll()).object();
}

GlobalStorageObject::~GlobalStorageObject() {
    update();
}

void GlobalStorageObject::update() {
    if (!m_isModified)
        return;
    m_storageFile.resize(0);
    m_storageFile.write(QJsonDocument(m_jsonStorage)
                            .toJson(
#ifdef QT_DEBUG
                                QJsonDocument::Indented
#else
                                QJsonDocument::Compact
#endif
                                ));
    m_isModified = false;
}

void GlobalStorageObject::setItem(const QString &key, const QString &value) {
    m_jsonStorage.insert(key, value);
    m_isModified = true;
}

QJSValue GlobalStorageObject::getItem(const QString &key) {
    auto value = m_jsonStorage.value(key);
    if (value.isString()) {
        return value.toString();
    } else {
        return QJSValue::NullValue;
    }
}

void GlobalStorageObject::removeItem(const QString &key) {
    m_jsonStorage.remove(key);
    m_isModified = true;
}
