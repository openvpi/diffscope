#include "GlobalRegistryObject.h"
#include "GlobalRegistryObject_p.h"

#include <QJSEngine>

#include "GlobalObject.h"

GlobalRegistryObject::GlobalRegistryObject(GlobalObject *global) : QObject(global), d(new GlobalRegistryObjectPrivate) {
}

GlobalRegistryObject::~GlobalRegistryObject() = default;

QStringList GlobalRegistryObject::scripts() const {
    return d->scriptDict.keys();
}
QJSValue GlobalRegistryObject::scriptConstructor(const QString &id) const {
    return d->scriptDict.value(id);
}

void GlobalRegistryObject::clearRegistry() {
    d->scriptDict.clear();
}

void GlobalRegistryObject::registerScript(const QJSValue &scriptConstructor) {
    if (!scriptConstructor.property("manifest").isCallable())
        return jsGlobal->engine()->throwError(QJSValue::TypeError, "'manifest' is not a function");
    auto manifest = scriptConstructor.property("manifest").call();
    if (!manifest.property("id").isString())
        return jsGlobal->engine()->throwError(QJSValue::TypeError, "Invalid return value of manifest()");;
    d->scriptDict.insert(manifest.property("id").toString(), scriptConstructor);
}
