#include "ObjectWrapper.h"

#include <QDebug>
#include <QJSEngine>
#include <QMetaMethod>

QStringList ObjectWrapper::qWidgetGeneralKeys() {
    return {"enabled", "visible", "toolTip"};
}

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
        QJSValue targetObj = jsObj;
        if (!jsObj.hasProperty(key)) {
            if (!jsObj.property("_subObject").hasProperty(key)) {
                qWarning() << "ObjectWrapper: No property named" << key;
                continue;
            }
            targetObj = jsObj.property("_subObject");
        }
        auto desc = engine->newObject();
        desc.setProperty("configurable", true);
        desc.setProperty("enumerable", true);
        auto accessorDesc = engine->newQObject(new JSValueAccessorDescriptor(targetObj, key));
        desc.setProperty("get", accessorDesc.property("get"));
        desc.setProperty("set", accessorDesc.property("set"));
        Object.property("defineProperty").call({wrapped, key, desc});
    }

    for (const auto &key : excludedKeys) {
        wrapped.deleteProperty(key);
    }

    return wrapped;
}

void ObjectWrapper::addAccessorPropertyImpl(QJSValue wrapped, QJSEngine *engine, const QString &key,
                                            JSValueCustomAccessorDescriptor *descriptorObject) {
    auto desc = engine->newObject();
    desc.setProperty("configurable", true);
    desc.setProperty("enumerable", true);
    auto accessorDesc = engine->newQObject(descriptorObject);
    desc.setProperty("get", accessorDesc.property("get"));
    desc.setProperty("set", accessorDesc.property("set"));
    engine->globalObject().property("Object").property("defineProperty").call({wrapped, key, desc});
}

QJSValue ObjectWrapper::toUint8Array(const QByteArray &ba, QJSEngine *engine) {
    auto uint8Array = engine->globalObject().property("Uint8Array").callAsConstructor({ba.size()});
    fillUint8Array(ba, uint8Array);
    return uint8Array;
}

void ObjectWrapper::fillUint8Array(const QByteArray &ba, QJSValue uint8Array) {
    auto size = uint8Array.property("length").toInt();
    for (int i = 0; i < size; i++) {
        uint8Array.setProperty(i, ba[i]);
    }
}

QByteArray ObjectWrapper::fromUint8Array(const QJSValue &uint8Array, QJSEngine *engine) {
    QJSValue data = uint8Array;
    if (engine->globalObject().property("ArrayBuffer").property("isView").call({uint8Array}).toBool()) {
        data = uint8Array.property("buffer");
    }
    data = engine->globalObject().property("Uint8Array").callAsConstructor({data});
    auto size = data.property("length").toInt();
    QByteArray ba(size, Qt::Uninitialized);
    for (int i = 0; i < size; i++) {
        ba[i] = data.property(i).toInt();
    }
    return ba;
}
