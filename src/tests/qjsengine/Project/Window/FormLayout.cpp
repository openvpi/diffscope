#include "FormLayout.h"

#include <QWidget>
#include <QFormLayout>
#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../ProjectWindowObject.h"

FormLayout::FormLayout(QWidget *parent) : QFormLayout(parent) {
}

void FormLayout::addRow(const QString &label, const QJSValue &jsWidget) {
    auto *widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::addRow(label, widget);
        return;
    }
    auto *layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::addRow(label, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::addElement(const QJSValue &jsWidget) {
    auto *widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::addRow(widget);
        return;
    }
    auto *layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::addRow(layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::insertRow(int row, const QString &label, const QJSValue &jsWidget) {
    auto *widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::insertRow(row, label, widget);
        return;
    }
    auto *layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::insertRow(row, label, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::insertElement(int row, const QJSValue &jsWidget) {
    auto *widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::insertRow(row, widget);
        return;
    }
    auto *layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::insertRow(row, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}
