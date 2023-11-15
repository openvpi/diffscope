#include "Radio.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue Radio::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() +
                                       QStringList{"autoExclusive", "checkable", "checked", "text"});
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, clicked);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, pressed);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, released);
    return obj;
}

void Radio::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                   const QJSValue &children,
                                                   const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    if (children.property(0).isString()) {
        setText(children.property(0).toString());
    }
}

Radio::Radio(QWidget *parent) : QRadioButton(parent) {
}
