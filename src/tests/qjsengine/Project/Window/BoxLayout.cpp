#include "BoxLayout.h"

#include <QJSEngine>
#include <QWidget>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"
#include "../ProjectWindowObject.h"

QJSValue BoxLayout::createScriptObject() {
    return ObjectWrapper::wrap(this, jsGlobal->engine(),
                               {"direction", "addElement", "addSpacing", "addStretch", "addStrut", "insertElement",
                                "insertSpacing", "insertStretch", "spacing", "count"});
}

void BoxLayout::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                       const QJSValue &children,
                                                       const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    int childrenCount = children.property("length").toInt();
    for (int i = 0; i < childrenCount; i++) {
        QJSValue child = children.property(i);
        if (child.property("tag").toString() == "box-layout-spacing") {
            addSpacing(child.property("attributes").property("value").toInt());
        } else if (child.property("tag").toString() == "box-layout-stretch") {
            addStretch(child.property("attributes").property("value").toInt());
        } else if (child.property("tag").toString() == "box-layout-strut") {
            addStrut(child.property("attributes").property("value").toInt());
        } else {
            addElement(renderer(child), child.property("attributes").property("box-layout-stretch").toInt(),
                       child.property("attributes").property("box-layout-alignment").toInt());
        }
    }
}

BoxLayout::BoxLayout(QWidget *parent) : QBoxLayout(TopToBottom, parent) {
}

int BoxLayout::direction() {
    return QBoxLayout::direction();
}

void BoxLayout::setDirection(int direction) {
    QBoxLayout::setDirection(QBoxLayout::Direction(direction));
}

void BoxLayout::addElement(const QJSValue &jsWidget, int stretch, int alignment) {
    auto widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QBoxLayout::addWidget(widget, stretch, Qt::Alignment(alignment));
        return;
    }
    auto layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QBoxLayout::addLayout(layout, stretch);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void BoxLayout::addSpacing(int size) {
    QBoxLayout::addSpacing(size);
}

void BoxLayout::addStretch(int stretch) {
    QBoxLayout::addStretch(stretch);
}

void BoxLayout::addStrut(int size) {
    QBoxLayout::addStrut(size);
}

void BoxLayout::insertElement(int index, const QJSValue &jsWidget, int stretch, int alignment) {
    auto widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QBoxLayout::insertWidget(index, widget, stretch, Qt::Alignment(alignment));
        return;
    }
    auto layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QBoxLayout::insertLayout(index, layout, stretch);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void BoxLayout::insertSpacing(int index, int size) {
    QBoxLayout::insertSpacing(index, size);
}

void BoxLayout::insertStretch(int index, int stretch) {
    QBoxLayout::insertStretch(index, stretch);
}
