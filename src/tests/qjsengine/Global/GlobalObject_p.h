#ifndef DIFFSCOPE_GLOBALOBJECT_P_H
#define DIFFSCOPE_GLOBALOBJECT_P_H

#include <QJSEngine>

#include "GlobalRegistryObject.h"

class GlobalObjectPrivate {
public:
    QJSEngine *engine;

    GlobalRegistryObject *registry;
    QJSValue registryObject;
    QJSValue storageObject;
};

#endif // DIFFSCOPE_GLOBALOBJECT_P_H
