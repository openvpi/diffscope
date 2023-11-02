#include "GlobalStorageObject.h"
#include "GlobalStorageObject_p.h"

#include "GlobalObject.h"

#include <QJsonDocument>

GlobalStorageObject::GlobalStorageObject(GlobalObject *global, const QString &filename) : QObject(global), d(new GlobalStorageObjectPrivate) {
    d->storageFile.setFileName(filename);
    d->storageFile.open(QFile::ReadWrite);
    d->jsonStorage = QJsonDocument::fromJson(d->storageFile.readAll()).object();
}

GlobalStorageObject::~GlobalStorageObject() {
    update();
}

void GlobalStorageObject::update() {
    if (!d->isModified)
        return;
    d->storageFile.resize(0);
    d->storageFile.write(QJsonDocument(d->jsonStorage).toJson(
#ifdef QT_DEBUG
        QJsonDocument::Indented
#else
        QJsonDocument::Compact
#endif
    ));
    d->isModified = false;
}

void GlobalStorageObject::setItem(const QString &key, const QString &value) {
    d->jsonStorage.insert(key, value);
    d->isModified = true;
}

QJSValue GlobalStorageObject::getItem(const QString &key) {
    auto value = d->jsonStorage.value(key);
    if (value.isString()) {
        return value.toString();
    } else {
        return QJSValue::NullValue;
    }
}

void GlobalStorageObject::removeItem(const QString &key) {
    d->jsonStorage.remove(key);
    d->isModified = true;
}
