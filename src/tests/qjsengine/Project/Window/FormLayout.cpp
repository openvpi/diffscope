#include "FormLayout.h"

#include <QWidget>
#include <QFormLayout>
#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"
#include "../ProjectWindowObject.h"

QJSValue FormLayout::createScriptObject() {
    return ObjectWrapper::wrap(
        this, jsGlobal->engine(),
        {"addRow", "addElement", "insertRow", "insertElement", "horizontalSpacing", "verticalSpacing", "rowCount"});
}
void FormLayout::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                        const QJSValue &children,
                                                        const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    int childrenCount = children.property("length").toInt();
    for (int i = 0; i < childrenCount; i++) {
        auto child = children.property(i);
        if (child.property("attributes").property("form-layout-label").isString()) {
            addRow(child.property("attributes").property("form-layout-label").toString(), renderer(child));
        } else {
            addElement(renderer(child));
        }
    }
}

FormLayout::FormLayout(QWidget *parent) : QFormLayout(parent) {
}

void FormLayout::addRow(const QString &label, const QJSValue &jsWidget) {
    auto *widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::addRow(label, widget);
        return;
    }
    auto *layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::addRow(label, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::addElement(const QJSValue &jsWidget) {
    auto *widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::addRow(widget);
        return;
    }
    auto *layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::addRow(layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::insertRow(int row, const QString &label, const QJSValue &jsWidget) {
    auto *widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::insertRow(row, label, widget);
        return;
    }
    auto *layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::insertRow(row, label, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::insertElement(int row, const QJSValue &jsWidget) {
    auto *widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QFormLayout::insertRow(row, widget);
        return;
    }
    auto *layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QFormLayout::insertRow(row, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}
