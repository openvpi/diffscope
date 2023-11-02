#include "ProjectObject.h"
#include "ProjectObject_p.h"

#include <QDebug>
#include <QWidget>
#include <QJSEngine>

#include "../Global/GlobalObject.h"
#include "../Global/GlobalRegistryObject.h"
#include "ProjectWindowObject.h"

ProjectObject::ProjectObject(QWidget *window) : QObject(window), d(new ProjectObjectPrivate{window}) {
    d->thisObject = jsGlobal->engine()->newQObject(this);
    d->windowObject = jsGlobal->engine()->newQObject(new ProjectWindowObject(this));
}

ProjectObject::~ProjectObject() = default;

QWidget *ProjectObject::window() const {
    return d->win;
}

QJSValue ProjectObject::invoke(const QString &id, int index) {
    auto scriptConstructor = jsGlobal->registry()->scriptConstructor(id);
    if (scriptConstructor.isUndefined())
        return QJSValue::UndefinedValue;
    auto script = scriptConstructor.callAsConstructor({d->thisObject});
    if (script.isError())
        return script;
    if (!script.property("prepare").isCallable())
        return jsGlobal->engine()->newErrorObject(QJSValue::TypeError, "'prepare' is not a function");
    auto isAccepted = script.property("prepare").callWithInstance(script, {index});
    if (!isAccepted.toBool())
        return QJSValue::UndefinedValue;
    if (!script.property("main").isCallable())
        return jsGlobal->engine()->newErrorObject(QJSValue::TypeError, "'main' is not a function");
    // TODO check whether the script requires transaction
    auto ret = script.property("main").callWithInstance(script);
    if (ret.isError())
        return ret;
    return QJSValue::UndefinedValue;
}

QJSValue ProjectObject::jsWindow() const {
    return d->windowObject;
}
