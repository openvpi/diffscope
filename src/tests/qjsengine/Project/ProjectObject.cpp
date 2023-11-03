#include "ProjectObject.h"

#include <QDebug>
#include <QJSEngine>
#include <QWidget>

#include "../Global/GlobalObject.h"
#include "../Global/GlobalRegistryObject.h"
#include "../Global/GlobalStorageObject.h"
#include "ProjectWindowObject.h"

ProjectObject::ProjectObject(QWidget *window)
    : QObject(window), m_win(window), m_thisObject(JS_QOBJ(this)),
      m_windowObject(JS_QOBJ(new ProjectWindowObject(this))) {
}

ProjectObject::~ProjectObject() = default;

QWidget *ProjectObject::window() const {
    return m_win;
}

QJSValue ProjectObject::invoke(const QString &id, int index) {
    auto scriptConstructor = jsGlobal->registry()->scriptConstructor(id);
    if (scriptConstructor.isUndefined())
        return QJSValue::UndefinedValue;
    auto script = scriptConstructor.callAsConstructor({m_thisObject});
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
    jsGlobal->storage()->update();
    return QJSValue::UndefinedValue;
}

QJSValue ProjectObject::jsWindow() const {
    return m_windowObject;
}
