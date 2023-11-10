#include "BoxLayout.h"

#include <QJSEngine>
#include <QWidget>

#include "../../Global/GlobalObject.h"

BoxLayout::BoxLayout(QWidget *parent) : QBoxLayout(TopToBottom, parent) {
}

int BoxLayout::direction() {
    return QBoxLayout::direction();
}

void BoxLayout::setDirection(int direction) {
    QBoxLayout::setDirection(QBoxLayout::Direction(direction));
}

void BoxLayout::addElement(const QJSValue &jsWidget, int stretch, int alignment) {
    auto widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QBoxLayout::addWidget(widget, stretch, Qt::Alignment(alignment));
        return;
    }
    auto layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
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
    auto widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QBoxLayout::insertWidget(index, widget, stretch, Qt::Alignment(alignment));
        return;
    }
    auto layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
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

