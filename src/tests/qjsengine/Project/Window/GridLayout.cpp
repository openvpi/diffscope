#include "GridLayout.h"

#include <QJSEngine>
#include <QWidget>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue GridLayout::createScriptObject() {
    return ObjectWrapper::wrap(this, jsGlobal->engine(),
                               {"addElement", "addElement", "rowMinimumHeight", "columnMinimumWidth",
                                "setRowMinimumHeight", "setColumnMinimumWidth", "rowStretch", "columnStretch",
                                "setRowStretch", "setColumnStretch", "horizontalSpacing", "verticalSpacing", "rowCount",
                                "columnCount", "count", "originCorner"});
}
void GridLayout::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                        const QJSValue &children,
                                                        const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    int childrenCount = children.property("length").toInt();
    for (int i = 0; i < childrenCount; i++) {
        auto child = children.property(i);
        if (child.property("attributes").hasProperty("grid-layout-row-span"))
            addElement(renderer(child), child.property("attributes").property("grid-layout-row").toInt(),
                       child.property("attributes").property("grid-layout-column").toInt(),
                       child.property("attributes").property("grid-layout-row-span").toInt(),
                       child.property("attributes").property("grid-layout-column-span").toInt(),
                       child.property("attributes").property("grid-layout-alignment").toInt());
        else
            addElement(renderer(child), child.property("attributes").property("grid-layout-row").toInt(),
                       child.property("attributes").property("grid-layout-column").toInt(),
                       child.property("attributes").property("grid-layout-alignment").toInt());
    }
}

GridLayout::GridLayout(QWidget *parent) : QGridLayout(parent) {
}

void GridLayout::addElement(const QJSValue &jsWidget, int row, int column, int alignment) {
    auto widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QGridLayout::addWidget(widget, row, column, Qt::Alignment(alignment));
        return;
    }
    auto layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QGridLayout::addLayout(layout, row, column, Qt::Alignment(alignment));
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void GridLayout::addElement(const QJSValue &jsWidget, int row, int column, int rowSpan, int columnSpan, int alignment) {
    auto widget = ObjectWrapper::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        QGridLayout::addWidget(widget, row, column, rowSpan, columnSpan, Qt::Alignment(alignment));
        return;
    }
    auto layout = ObjectWrapper::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        QGridLayout::addLayout(layout, row, column, rowSpan, columnSpan, Qt::Alignment(alignment));
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

int GridLayout::rowMinimumHeight(int row) {
    return QGridLayout::rowMinimumHeight(row);
}

int GridLayout::columnMinimumWidth(int column) {
    return QGridLayout::columnMinimumWidth(column);
}

void GridLayout::setRowMinimumHeight(int row, int minSize) {
    QGridLayout::setRowMinimumHeight(row, minSize);
}

void GridLayout::setColumnMinimumWidth(int column, int minSize) {
    QGridLayout::setColumnMinimumWidth(column, minSize);
}

int GridLayout::rowStretch(int row) {
    return QGridLayout::rowStretch(row);
}

int GridLayout::columnStretch(int column) {
    return QGridLayout::columnStretch(column);
}

void GridLayout::setRowStretch(int row, int stretch) {
    QGridLayout::setRowStretch(row, stretch);
}

void GridLayout::setColumnStretch(int column, int stretch) {
    QGridLayout::setColumnStretch(column, stretch);
}

void GridLayout::setOriginCorner(int corner) {
    QGridLayout::setOriginCorner(Qt::Corner(corner));
}

int GridLayout::originCorner() const {
    return QGridLayout::originCorner();
}
