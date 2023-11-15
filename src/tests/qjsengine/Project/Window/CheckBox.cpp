#include "CheckBox.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue CheckBox::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() + QStringList{
                                                                             "tristate",
                                                                             "checkState",
                                                                             "autoExclusive",
                                                                             "checkable",
                                                                             "checked",
                                                                             "text",
                                                                         });
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, clicked);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, pressed);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, released);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, stateChanged);
    return obj;
}
void CheckBox::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                      const QJSValue &children,
                                                      const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    if (children.property(0).isString()) {
        setText(children.property(0).toString());
    }
}

CheckBox::CheckBox(QWidget *parent) : QCheckBox(parent) {
}

void CheckBox::setCheckState(int state) {
    QCheckBox::setCheckState(Qt::CheckState(state));
}

int CheckBox::checkState() const {
    return QCheckBox::checkState();
}
