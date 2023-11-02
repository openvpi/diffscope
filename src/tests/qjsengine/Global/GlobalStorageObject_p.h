#ifndef DIFFSCOPE_GLOBALSTORAGEOBJECT_P_H
#define DIFFSCOPE_GLOBALSTORAGEOBJECT_P_H

#include "GlobalStorageObject.h"

#include <QFile>
#include <QJsonObject>

class GlobalStorageObjectPrivate {
public:
    QFile storageFile;
    QJsonObject jsonStorage;
    bool isModified = false;
};

#endif // DIFFSCOPE_GLOBALSTORAGEOBJECT_P_H
