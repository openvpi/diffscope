#include "GlobalRegistryObject.h"

#include <QJSEngine>
#include <QDebug>

#include "GlobalObject.h"
#include "Console.h"

#include "../BuiltIn/TransposeScript.h"

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

void GlobalRegistryObject::registerScriptImplementationImpl(const QMetaObject *impl, const QJSValue &manifest) {
    auto ret = m_builtInScriptHelper.call({jsGlobal->engine()->newQMetaObject(impl), manifest});
    if (ret.isError())
        jsGlobal->console()->printUncaughtError(ret);
}

void GlobalRegistryObject::registerBuiltInScripts() {
    m_builtInScriptHelper = jsGlobal->load(":/scripts/builtInScriptHelper.js");
    registerScriptImplementation<TransposeScript>();
}
