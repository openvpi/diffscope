#include "StackedLayout.h"

#include <QJSEngine>
#include <QWidget>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue StackedLayout::createScriptObject() {
    return ObjectWrapper::wrap(this, jsGlobal->engine(), {"addElement", "insertElement", "count", "currentIndex"});
}
void StackedLayout::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                           const QJSValue &children,
                                                           const std::function<QJSValue(const QJSValue &)> &renderer) {
    int childrenCount = children.property("length").toInt();
    for (int i = 0; i < childrenCount; i++) {
        auto child = children.property(i);
        addElement(renderer(child));
    }
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
}

StackedLayout::StackedLayout(QWidget *parent) : QStackedLayout(parent) {
}

void StackedLayout::addElement(const QJSValue &jsWidget) {
    auto widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        addWidget(widget);
        return;
    }
    auto layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        auto w = new QWidget;
        w->setLayout(layout);
        addWidget(w);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void StackedLayout::insertElement(int index, const QJSValue &jsWidget) {
    auto widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        insertWidget(index, widget);
        return;
    }
    auto layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        auto w = new QWidget;
        w->setLayout(layout);
        insertWidget(index, w);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}
