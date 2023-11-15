#include "Label.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue Label::createScriptObject() {
    return ObjectWrapper::wrap(this, jsGlobal->engine(), ObjectWrapper::qWidgetGeneralKeys() + QStringList{"text"});
}
void Label::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                   const QJSValue &children,
                                                   const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    if (children.property(0).isString()) {
        setText(children.property(0).toString());
    }
}

Label::Label(QWidget *parent) : QLabel(parent) {
}
