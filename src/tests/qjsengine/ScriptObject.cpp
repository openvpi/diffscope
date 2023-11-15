#include "ScriptObject.h"

#include "Global/GlobalObject.h"
#include <QJSEngine>

void ScriptDescriptiveObject::configureThisScriptObjectByDescription(
    QJSValue wrappedObject, const QJSValue &attributes, const QJSValue &children,
    const std::function<QJSValue(const QJSValue &)> &renderer) {
    auto attributeKeys = jsGlobal->engine()
                             ->globalObject()
                             .property("Object")
                             .property("keys")
                             .call({wrappedObject})
                             .toVariant()
                             .toStringList();
    for (const auto &key : attributeKeys) {
        wrappedObject.setProperty(key, attributes.property(key));
    }
}
