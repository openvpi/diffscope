#include "FormLayout.h"

#include <QWidget>
#include <QFormLayout>
#include <QJSEngine>

#include "../../Global/GlobalObject.h"

FormLayout::FormLayout(QWidget *win) : QObject(win), m_layout(new QFormLayout), m_layoutObject(JS_QOBJ(m_layout)) {
}

void FormLayout::addRow(const QString &label, const QJSValue &jsWidget) {
    auto *widget = qobject_cast<QWidget *>(jsWidget.property("_p").toQObject());
    if (widget) {
        m_layout->addRow(label, widget);
        return;
    }
    auto *layout = qobject_cast<QLayout *>(jsWidget.property("_p").toQObject());
    if (layout) {
        m_layout->addRow(label, layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}

void FormLayout::addElement(const QJSValue &jsWidget) {
    auto *widget = qobject_cast<QWidget *>(jsWidget.property("_p").toQObject());
    if (widget) {
        m_layout->addRow(widget);
        return;
    }
    auto *layout = qobject_cast<QLayout *>(jsWidget.property("_p").toQObject());
    if (layout) {
        m_layout->addRow(layout);
        return;
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of layout content");
}
