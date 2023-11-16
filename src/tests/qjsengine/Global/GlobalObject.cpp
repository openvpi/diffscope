#include "GlobalObject.h"

#include <QFile>
#include <QJSEngine>

#include "GlobalRegistryObject.h"
#include "GlobalStorageObject.h"
#include "../ObjectWrapper.h"

static GlobalObject *m_instance = nullptr;

GlobalObject::GlobalObject(QObject *parent)
    : QObject(parent), m_engine(new QJSEngine(this)), m_registry(new GlobalRegistryObject(this)),
      m_registryObject(m_engine->newQObject(m_registry)),
      m_storage(new GlobalStorageObject(this, "D:/a.json")), // TODO file name
      m_storageObject(m_engine->newQObject(m_storage)) {
    m_instance = this;
    m_engine->installExtensions(QJSEngine::ConsoleExtension);
    m_engine->globalObject().setProperty("$", JS_QOBJ(this));
    defineEnum("OpenMode", {
                               {"NotOpen", 0x0001},
                               {"ReadOnly", 0x0001},
                               {"WriteOnly", 0x0002},
                               {"ReadWrite", 0x0003},
                               {"Append", 0x0004},
                               {"Truncate", 0x0008},
                               {"Text", 0x0010},
                           });
}

GlobalObject::~GlobalObject() {
    m_instance = nullptr;
}

GlobalObject *GlobalObject::instance() {
    return m_instance;
}

QJSEngine *GlobalObject::engine() const {
    return m_engine;
}

QJSValue GlobalObject::load(const QString &scriptFilename) {
    QFile f(scriptFilename);
    if (!f.open(QFile::ReadOnly))
        return m_engine->newErrorObject(QJSValue::URIError,
                                        QString("Script file does not exist: '%1'").arg(scriptFilename));
    auto ret = m_engine->evaluate(f.readAll(), scriptFilename);
    if (ret.isError())
        return ret;
    return QJSValue::UndefinedValue;
}

GlobalRegistryObject *GlobalObject::registry() const {
    return m_registry;
}

GlobalStorageObject *GlobalObject::storage() const {
    return m_storage;
}

QJSValue GlobalObject::jsRegistry() const {
    return m_registryObject;
}

QJSValue GlobalObject::jsStorage() const {
    return m_storageObject;
}

void GlobalObject::defineEnum(const QString &enumName, const QList<JSEnumEntry> &entries) {
    auto enumObj = m_engine->newObject();
    int index = 0;
    for (const auto &entry: entries) {
        if (entry.i != -1)
            index = entry.i;
        enumObj.setProperty(index, entry.s);
        enumObj.setProperty(entry.s, index++);
    }
    m_engine->globalObject().property("Object").property("freeze").call({enumObj});
    m_engine->globalObject().property("$").setProperty(enumName, enumObj);
}
