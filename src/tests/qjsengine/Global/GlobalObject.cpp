#include "GlobalObject.h"

#include <QFile>
#include <QJSEngine>
#include <QMessageBox>

#include "../ObjectWrapper.h"
#include "GlobalRegistryObject.h"
#include "GlobalStorageObject.h"
#include "TextDecoder.h"
#include "TextEncoder.h"
#include "Console.h"

static GlobalObject *m_instance = nullptr;

GlobalObject::GlobalObject(QObject *parent)
    : QObject(parent), m_engine(new QJSEngine(m_instance = this)), m_registry(new GlobalRegistryObject(this)), m_console(new Console(this)),
      m_registryObject(m_engine->newQObject(m_registry)),
      m_storage(new GlobalStorageObject(this, "D:/a.json")), // TODO file name
      m_storageObject(m_engine->newQObject(m_storage)) {

    installTextCodec();
    installConsole();

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

void GlobalObject::installConsole() {
    m_engine->evaluate("(c, global) => {\n"
                       "    let console = {};\n"
                       "    console.assert = (a, ...b) => c.assert(a, b);\n"
                       "    console.clear = () => c.clear();\n"
                       "    console.count = (a) => c.count(a);\n"
                       "    console.countReset = (a) => c.countReset(a);\n"
                       "    console.debug = (...a) => c.debug(a);\n"
                       "    console.error = (...a) => c.error(a);\n"
                       "    console.exception = (...a) => c.exception(a);\n"
                       "    console.info = (...a) => c.info(a);\n"
                       "    console.log = (...a) => c.log(a);\n"
                       "    console.time = (a) => c.time(a);\n"
                       "    console.timeEnd = (a) => c.timeEnd(a);\n"
                       "    console.timeLog = (a, ...b) => c.timeLog(a, b);\n"
                       "    console.trace = (...a) => c.trace(a);\n"
                       "    console.warn = (...a) => c.warn(a);\n"
                       "    Object.freeze(console);\n"
                       "    global.console = console;\n"
                       "}").call({JS_QOBJ(m_console), m_engine->globalObject()});
}

void GlobalObject::installTextCodec() {
    m_engine->globalObject().setProperty("TextDecoder", m_engine->newQMetaObject<TextDecoder>());
    m_engine->globalObject().setProperty("TextEncoder", m_engine->newQMetaObject<TextEncoder>());
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

QString GlobalObject::stackTrace(int depth) {
    return m_engine->evaluate("throw new Error()").property("stack").toString().split("\n").mid(1 + depth).join("\n");
}

QString GlobalObject::fileTrace(int depth) {
    auto stack = stackTrace().split("\n")[depth];
    return stack.mid(stack.indexOf('@') + 1);
}

QJSValue GlobalObject::load(const QString &scriptFilename) {
    QFile f(scriptFilename);
    if (!f.open(QFile::ReadOnly)) {
        auto err = m_engine->newErrorObject(QJSValue::URIError,
                                            QString("Script file does not exist: '%1'").arg(scriptFilename));
        m_console->printUncaughtError(err);
        return err;
    }
    auto ret = m_engine->evaluate(f.readAll(), scriptFilename);
    if (ret.isError()) {
        m_console->printUncaughtError(ret);
    }
    return ret;
}

GlobalRegistryObject *GlobalObject::registry() const {
    return m_registry;
}

GlobalStorageObject *GlobalObject::storage() const {
    return m_storage;
}

Console *GlobalObject::console() const {
    return m_console;
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

void GlobalObject::pause() {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("JavaScript Pause"));
    msgBox.setText(tr("Script execution is paused.\nStack trace:\n") + stackTrace());
    msgBox.addButton(tr("Continue"), QMessageBox::AcceptRole);
    msgBox.exec();
}
