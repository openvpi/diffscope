#ifndef DIFFSCOPE_GLOBALREGISTRYOBJECT_P_H
#define DIFFSCOPE_GLOBALREGISTRYOBJECT_P_H

#include "GlobalRegistryObject.h"
#include <QHash>

class GlobalRegistryObjectPrivate {
public:
    QHash<QString, QJSValue> scriptDict;
};

#endif // DIFFSCOPE_GLOBALREGISTRYOBJECT_P_H
