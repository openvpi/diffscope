#include "ObjectWrapper.h"

#include <QDebug>
#include <QJSEngine>
#include <QMetaMethod>

QJSValue ObjectWrapper::wrap(QObject *obj, QJSEngine *engine, const QStringList &includedKeys,
                             const QStringList &excludedKeys) {
    auto Object = engine->globalObject().property("Object");
    auto wrapped = engine->newObject();

    auto jsObj = engine->newQObject(obj);
    auto jsObjDesc = engine->newObject();
    jsObjDesc.setProperty("value", jsObj);
    Object.property("defineProperty").call({wrapped, "_p", jsObjDesc});

    auto keys = includedKeys;
    if (keys.isEmpty())
        keys = Object.property("keys").call({jsObj}).toVariant().toStringList();

    for (const auto &key : keys) {
        auto desc = engine->newObject();
        desc.setProperty("configurable", true);
        desc.setProperty("enumerable", true);
        auto accessorDesc = engine->newQObject(new JSValueAccessorDescriptor(jsObj, key));
        desc.setProperty("get", accessorDesc.property("get"));
        desc.setProperty("set", accessorDesc.property("set"));
        Object.property("defineProperty").call({wrapped, key, desc});
    }

    for (const auto &key : excludedKeys) {
        wrapped.deleteProperty(key);
    }

    return wrapped;
}
