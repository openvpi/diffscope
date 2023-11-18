#include "ProjectObject.h"

#include <QApplication>
#include <QDebug>
#include <QJSEngine>
#include <QWidget>

#include "../Global/GlobalObject.h"
#include "../Global/Console.h"
#include "../Global/GlobalRegistryObject.h"
#include "../Global/GlobalStorageObject.h"
#include "ProjectWindowObject.h"

ProjectObject::ProjectObject(QWidget *window)
    : QObject(window), m_win(window), m_thisObject(JS_QOBJ(this)),
      m_windowQObject(new ProjectWindowObject(this)),
      m_windowObject(JS_QOBJ(m_windowQObject)) {
}

ProjectObject::~ProjectObject() = default;

QWidget *ProjectObject::window() const {
    return m_win;
}

QJSValue ProjectObject::invoke(const QString &id, int index) {
    auto ret = invokeImpl(id, index);
    finishProgress();
    m_windowQObject->finalizeScriptScope();
    return ret;
}

QJSValue ProjectObject::invokeImpl(const QString &id, int index) {
    auto scriptConstructor = jsGlobal->registry()->scriptConstructor(id);
    if (scriptConstructor.isUndefined())
        return QJSValue::UndefinedValue;
    auto script = scriptConstructor.callAsConstructor({m_thisObject});
    if (script.isError()) {
        jsGlobal->console()->printUncaughtError(script);
        return script;
    }
    if (!script.property("prepare").isCallable())
        return jsGlobal->engine()->newErrorObject(QJSValue::TypeError, "'prepare' is not a function");
    auto isAccepted = script.property("prepare").callWithInstance(script, {index});
    if (isAccepted.isError()) {
        jsGlobal->console()->printUncaughtError(isAccepted);
        return isAccepted;
    }
    if (!isAccepted.toBool())
        return QJSValue::UndefinedValue;
    if (!script.property("main").isCallable())
        return jsGlobal->engine()->newErrorObject(QJSValue::TypeError, "'main' is not a function");
    // TODO check whether the script requires transaction
    auto ret = script.property("main").callWithInstance(script);
    if (ret.isError()) {
        jsGlobal->console()->printUncaughtError(ret);
        return ret;
    }
    jsGlobal->storage()->update();
    return QJSValue::UndefinedValue;
}

QJSValue ProjectObject::jsWindow() const {
    return m_windowObject;
}

void ProjectObject::startProgress(const QString &title, int maximum) {
    m_isProgressStarted = true;
    emit progressStarted(title, maximum);
}

void ProjectObject::updateProgress(int value) {
    if (m_isProgressStarted)
        emit progressValueChanged(value);
    qApp->processEvents();
}

void ProjectObject::finishProgress() {
    if (m_isProgressStarted)
        emit progressFinished();
    m_isProgressStarted = false;
}
