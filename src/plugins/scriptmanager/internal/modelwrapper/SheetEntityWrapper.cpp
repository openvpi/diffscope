#include "SheetEntityWrapper.h"

#include <QJSEngine>
#include <QFile>

namespace ScriptManager::Internal {
    SheetEntityWrapper::SheetEntityWrapper() = default;
    SheetEntityWrapper::~SheetEntityWrapper() = default;
    QJSValue SheetEntityWrapper::wrap(QJSEngine *engine) {
        static QHash<QJSEngine *, QJSValue> wrapperFuncDict;
        static QString wrapperFuncCode = ([] {
            QFile f(":/scriptmanager/modelwrapper/SheetEntityWrapper.js");
            Q_ASSERT(f.open(QIODevice::ReadOnly));
            return f.readAll();
        })();
        if (wrapperFuncDict.contains(engine)) {
            auto ret = wrapperFuncDict[engine].call({engine->newQObject(this)});
            Q_ASSERT(!ret.isError());
            return ret;
        }
        auto wrapperFunc = engine->evaluate(wrapperFuncCode);
        Q_ASSERT(wrapperFunc.isCallable());
        wrapperFuncDict.insert(engine, wrapperFunc);
        connect(engine, &QObject::destroyed, [=] {
            wrapperFuncDict.remove(engine);
        });
        auto ret = wrapperFunc.call({engine->newQObject(this)});
        Q_ASSERT(!ret.isError());
        return ret;
    }
}