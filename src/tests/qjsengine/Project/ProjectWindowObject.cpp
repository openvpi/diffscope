#include "ProjectWindowObject.h"
#include <QLayout>
#include <QWidget>

#include <QApplication>
#include <QComboBox>
#include <QJSEngine>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>

#include "../Global/GlobalObject.h"
#include "ProjectObject.h"

#include "../ObjectWrapper.h"
#include "Window/ComboBox.h"
#include "Window/Dialog.h"
#include "Window/FormLayout.h"
#include "Window/Slider.h"

ProjectWindowObject::ProjectWindowObject(ProjectObject *project) : QObject(project), m_project(project) {
}

ProjectWindowObject::~ProjectWindowObject() = default;

QWidget *ProjectWindowObject::window() const {
    return m_project->window();
}

void ProjectWindowObject::alert(const QString &message, const QString &title) {
    QMessageBox::information(m_project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

void ProjectWindowObject::warning(const QString &message, const QString &title) {
    QMessageBox::warning(m_project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

void ProjectWindowObject::critical(const QString &message, const QString &title) {
    QMessageBox::critical(m_project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

bool ProjectWindowObject::question(const QString &message, const QString &title) {
    return QMessageBox::question(m_project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title,
                                 message) == QMessageBox::Yes;
}

QJSValue ProjectWindowObject::createElement(const QString &tag) {
    QStringList qWidgetGeneralKeys = {"enabled", "visible", "toolTip"};
    if (tag == "dialog") {
        auto dlg = new Dialog(window());
        auto obj = ObjectWrapper::wrap(dlg, jsGlobal->engine(), {"content", "openDialog", "closeDialog"});
        return obj;
    }

    if (tag == "form-layout") {
        auto layout = new FormLayout;
        auto obj = ObjectWrapper::wrap(layout, jsGlobal->engine(), {"addRow", "addElement"});
        return obj;
    }

    if (tag == "label")
        return ObjectWrapper::wrap(new QLabel, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{"text"});

    if (tag == "button") {
        auto btn = new QPushButton;
        auto obj = ObjectWrapper::wrap(
            btn, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{"autoExclusive", "checkable", "checked", "text"});
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, clicked);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, pressed);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, released);
        return obj;
    }

    if (tag == "line-edit") {
        auto lineEdit = new QLineEdit;
        auto obj = ObjectWrapper::wrap(lineEdit, jsGlobal->engine(),
                                       qWidgetGeneralKeys + QStringList{"acceptableInput", "cursorPosition",
                                                                        "hasSelectedText", "inputMask", "maxLength",
                                                                        "placeholderText", "readOnly", "selectedText",
                                                                        "text", "clear", "copy", "selectAll"});
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, cursorPositionChanged);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, editingFinished);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, inputRejected);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, returnPressed);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, selectionChanged);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, textChanged);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, textEdited);
        return obj;
    }

    if (tag == "select") {
        auto comboBox = new ComboBox(window());
        auto obj = ObjectWrapper::wrap(comboBox, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{"currentIndex", "addOption", "optionAt"});
        ObjectWrapper::bindSignal(comboBox, QOverload<int>::of(&ComboBox::currentIndexChanged), obj, "currentIndexChanged");
        return obj;
    }

    if (tag == "slider") {
        auto slider = new Slider(window());
        auto obj = ObjectWrapper::wrap(slider, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{"maximum", "minimum", "pageStep", "singleStep", "value", "tickInterval", "hasTicks"});
        OBJECT_WRAPPER_BIND_SIGNAL(slider, obj, valueChanged);
        return obj;
    }

    if (tag == "spin-box") {
        auto spinBox = new QSpinBox;
        auto obj = ObjectWrapper::wrap(spinBox, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{"readOnly", "maximum", "minimum", "prefix", "singleStep", "suffix", "value"});
        ObjectWrapper::bindSignal(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), obj, "valueChanged");
        return obj;
    }

    if (tag == "double-spin-box") {
        auto spinBox = new QDoubleSpinBox;
        auto obj = ObjectWrapper::wrap(spinBox, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{"readOnly", "maximum", "minimum", "prefix", "singleStep", "suffix", "value", "decimals"});
        ObjectWrapper::bindSignal(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), obj, "valueChanged");
        return obj;
    }

    JS_THROW(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
    return QJSValue::UndefinedValue;
}
QWidget *ProjectWindowObject::getWidgetOfWrappedObject(const QJSValue &obj) {
    auto *widget = qobject_cast<QWidget *>(obj.property("_p").toQObject());
    if (widget)
        return widget;
    widget = qobject_cast<QWidget *>(obj.property("_p").property("_subObject").toQObject());
    return widget;
}
QLayout *ProjectWindowObject::getLayoutOfWrappedObject(const QJSValue &obj) {
    auto *layout = qobject_cast<QLayout *>(obj.property("_p").toQObject());
    if (layout)
        return layout;
    layout = qobject_cast<QLayout *>(obj.property("_p").property("_subObject").toQObject());
    return layout;
}