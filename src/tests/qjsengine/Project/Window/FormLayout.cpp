#include "FormLayout.h"

#include <QWidget>
#include <QFormLayout>
#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../ProjectWindowObject.h"

FormLayout::FormLayout(QWidget *parent) : QFormLayout(parent) {
}

void FormLayout::addRow(const QString &label, const QJSValue &jsWidget) {
    auto *widget = ProjectWindowObject::getWidgetOfWrappedObject(jsWidget);
    if (widget) {
        QFormLayout::addRow(label, widget);
        return;
    }
    auto *layout = ProjectWindowObject::getLayoutOfWrappedObject(jsWidget);
    if (layout) {
        QFormLayout::addRow(label, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::addElement(const QJSValue &jsWidget) {
    auto *widget = ProjectWindowObject::getWidgetOfWrappedObject(jsWidget);
    if (widget) {
        QFormLayout::addRow(widget);
        return;
    }
    auto *layout = ProjectWindowObject::getLayoutOfWrappedObject(jsWidget);
    if (layout) {
        QFormLayout::addRow(layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}
