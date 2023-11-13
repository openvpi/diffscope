#include "StackedLayout.h"

#include <QJSEngine>
#include <QWidget>

#include "../../Global/GlobalObject.h"

StackedLayout::StackedLayout(QWidget *parent) : QStackedLayout(parent) {
}

void StackedLayout::addElement(const QJSValue &jsWidget) {
    auto widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        addWidget(widget);
        return;
    }
    auto layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        auto w = new QWidget;
        w->setLayout(layout);
        addWidget(w);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void StackedLayout::insertElement(int index, const QJSValue &jsWidget) {
    auto widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        insertWidget(index, widget);
        return;
    }
    auto layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        auto w = new QWidget;
        w->setLayout(layout);
        insertWidget(index, w);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}
