#include "VectorEntityWrapper.h"

#include <QJSEngine>
#include <QFile>
#include <QHash>

namespace ScriptManager::Internal {

    VectorEntityWrapper::VectorEntityWrapper() = default;
    VectorEntityWrapper::~VectorEntityWrapper() = default;

    QJSValue VectorEntityWrapper::wrap(QJSEngine *engine) {
        static QHash<QJSEngine *, QJSValue> wrapperFuncDict;
        static QString wrapperFuncCode = ([] {
            QFile f(":/scriptmanager/modelwrapper/VectorEntityWrapper.js");
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