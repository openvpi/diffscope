#include "ProjectWindowObject.h"

#include <QApplication>
#include <QJSEngine>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

#include "../Global/GlobalObject.h"
#include "ProjectObject.h"

#include "../ObjectWrapper.h"
#include "Window/Dialog.h"
#include "Window/FormLayout.h"

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
    if (tag == "dialog")
        return JS_QOBJ(new Dialog(window()));

    if (tag == "form-layout")
        return JS_QOBJ(new FormLayout(window()));

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
    JS_THROW(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
    return QJSValue::UndefinedValue;
}
