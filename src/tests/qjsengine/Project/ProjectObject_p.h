#ifndef DIFFSCOPE_PROJECTOBJECT_P_H
#define DIFFSCOPE_PROJECTOBJECT_P_H

#include "ProjectObject.h"

#include <QHash>

class ProjectObjectPrivate {
public:
    QWidget *win;

    QHash<QString, QJSValue> scriptInstanceDict;

    QJSValue thisObject;
    QJSValue windowObject;
};

#endif // DIFFSCOPE_PROJECTOBJECT_P_H
