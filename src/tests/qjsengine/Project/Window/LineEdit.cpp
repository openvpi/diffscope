#include "LineEdit.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue LineEdit::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() +
                                       QStringList{"acceptableInput", "cursorPosition", "hasSelectedText", "inputMask",
                                                   "maxLength", "placeholderText", "readOnly", "selectedText", "text",
                                                   "clear", "copy", "selectAll"});
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, cursorPositionChanged);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, editingFinished);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, inputRejected);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, returnPressed);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, selectionChanged);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, textChanged);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, textEdited);
    return obj;
}
void LineEdit::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                      const QJSValue &children,
                                                      const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    if (children.property(0).isString()) {
        setText(children.property(0).toString());
    }
}

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) {
}
