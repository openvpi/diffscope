#include "GlobalRegistryObject.h"

#include <QJSEngine>

#include "GlobalObject.h"

GlobalRegistryObject::GlobalRegistryObject(GlobalObject *global) : QObject(global) {
}

GlobalRegistryObject::~GlobalRegistryObject() = default;

QStringList GlobalRegistryObject::scripts() const {
    return m_scriptDict.keys();
}
QJSValue GlobalRegistryObject::scriptConstructor(const QString &id) const {
    return m_scriptDict.value(id);
}

void GlobalRegistryObject::clearRegistry() {
    m_scriptDict.clear();
}

void GlobalRegistryObject::registerScript(const QJSValue &scriptConstructor) {
    if (!scriptConstructor.property("manifest").isCallable())
        return JS_THROW(QJSValue::TypeError, "'manifest' is not a function");
    auto manifest = scriptConstructor.property("manifest").call();
    if (!manifest.property("id").isString())
        return JS_THROW(QJSValue::TypeError, "Invalid return value of manifest()");
    m_scriptDict.insert(manifest.property("id").toString(), scriptConstructor);
}
