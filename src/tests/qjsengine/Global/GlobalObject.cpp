#include "GlobalObject.h"
#include "GlobalObject_p.h"

#include <QFile>
#include <QJSEngine>

static GlobalObject *m_instance = nullptr;

GlobalObject::GlobalObject(QObject *parent) : QObject(parent), d(new GlobalObjectPrivate) {
    m_instance = this;
    d->engine = new QJSEngine(this);
    d->registry = new GlobalRegistryObject(this);
    d->registryObject = d->engine->newQObject(d->registry);
    d->engine->globalObject().setProperty("$", d->engine->newQObject(this));
}

GlobalObject::~GlobalObject() {
    m_instance = nullptr;
}

GlobalObject *GlobalObject::instance() {
    return m_instance;
}

QJSEngine *GlobalObject::engine() const {
    return d->engine;
}

QJSValue GlobalObject::load(const QString &scriptFilename) {
    QFile f(scriptFilename);
    if (!f.open(QFile::ReadOnly))
        return d->engine->newErrorObject(QJSValue::URIError, QString("Script file does not exist: '%1'").arg(scriptFilename));
    auto ret = d->engine->evaluate(f.readAll(), scriptFilename);
    if (ret.isError())
        return ret;
    return QJSValue::UndefinedValue;
}

GlobalRegistryObject *GlobalObject::registry() const {
    return d->registry;
}

QJSValue GlobalObject::jsRegistry() const {
    return d->registryObject;
}

QJSValue GlobalObject::jsStorage() const {
    return d->storageObject;
}
