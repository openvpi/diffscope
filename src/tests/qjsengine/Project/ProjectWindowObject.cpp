#include "ProjectWindowObject.h"
#include <QLayout>
#include <QWidget>

#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QJSEngine>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>

#include "../Global/GlobalObject.h"
#include "ProjectObject.h"

#include "../ObjectWrapper.h"
#include "Window/BoxLayout.h"
#include "Window/ButtonGroup.h"
#include "Window/CheckBox.h"
#include "Window/ComboBox.h"
#include "Window/Dialog.h"
#include "Window/FileSelector.h"
#include "Window/FormLayout.h"
#include "Window/GridLayout.h"
#include "Window/Slider.h"
#include "Window/StackedLayout.h"

ProjectWindowObject::ProjectWindowObject(ProjectObject *project) : QObject(project), m_project(project) {
    jsGlobal->defineEnum("Alignment", {
                                          {"None",          0x0000},
                                          {"AlignLeft",     0x0001},
                                          {"AlignRight",    0x0002},
                                          {"AlignHCenter",  0x0004},
                                          {"AlignJustify",  0x0008},
                                          {"AlignTop",      0x0020},
                                          {"AlignBottom",   0x0040},
                                          {"AlignVCenter",  0x0080},
                                          {"AlignBaseline", 0x0100},
                                          {"AlignAbsolute", 0x0010}});
    jsGlobal->defineEnum("CheckState", {
                                           {"Unchecked"},
                                           {"PartiallyChecked"},
                                           {"Checked"}});
    jsGlobal->defineEnum("Corner", {
                                          {"TopLeftCorner"},
                                          {"TopRightCorner"},
                                          {"BottomLeftCorner"},
                                          {"BottomRightCorner"}});
    jsGlobal->defineEnum("Direction", {
                                          {"LeftToRight"},
                                          {"RightToLeft"},
                                          {"TopToBottom"},
                                          {"BottomToTop"}});
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

QJSValue ProjectWindowObject::createButtonGroup() {
    auto btnGrp = new ButtonGroup;
    auto obj = ObjectWrapper::wrap(btnGrp, jsGlobal->engine(), {
                                                                   "addButton",
                                                                   "checkedId",
                                                                   "id",
                                                                   "removeButton",
                                                                   "setId"
                                                               });
    OBJECT_WRAPPER_BIND_SIGNAL(btnGrp, obj, idClicked);
    OBJECT_WRAPPER_BIND_SIGNAL(btnGrp, obj, idPressed);
    OBJECT_WRAPPER_BIND_SIGNAL(btnGrp, obj, idReleased);
    OBJECT_WRAPPER_BIND_SIGNAL(btnGrp, obj, idToggled);
    return obj;
}

QJSValue ProjectWindowObject::createDialog() {
    return ObjectWrapper::wrap(new Dialog(window()), jsGlobal->engine(), {"content", "openDialog", "closeDialog"});
}

QJSValue ProjectWindowObject::createElement(const QString &tag) {
    QStringList qWidgetGeneralKeys = {"enabled", "visible", "toolTip"};
    if (tag == "box-layout")
        return ObjectWrapper::wrap(new BoxLayout, jsGlobal->engine(), {
                                                                          "direction",
                                                                          "addElement",
                                                                          "addSpacing",
                                                                          "addStretch",
                                                                          "addStrut",
                                                                          "insertElement",
                                                                          "insertSpacing",
                                                                          "insertStretch",
                                                                          "spacing",
                                                                          "count"});

    if (tag == "button") {
        auto btn = new QPushButton;
        auto obj = ObjectWrapper::wrap(
            btn, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{
                                                              "autoExclusive",
                                                              "checkable",
                                                              "checked",
                                                              "text"});
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, clicked);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, pressed);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, released);
        return obj;
    }

    if (tag == "check-box") {
        auto btn = new CheckBox;
        auto obj = ObjectWrapper::wrap(
            btn, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{
                                                              "tristate",
                                                              "checkState",
                                                              "autoExclusive",
                                                              "checkable",
                                                              "checked",
                                                              "text",});
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, clicked);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, pressed);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, released);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, stateChanged);
        return obj;
    }

    if (tag == "double-spin-box") {
        auto spinBox = new QDoubleSpinBox;
        auto obj = ObjectWrapper::wrap(spinBox, jsGlobal->engine(),
                                       qWidgetGeneralKeys + QStringList{
                                                                "readOnly",
                                                                "maximum",
                                                                "minimum",
                                                                "prefix",
                                                                "singleStep",
                                                                "suffix",
                                                                "value",
                                                                "decimals"});
        ObjectWrapper::bindSignal(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), obj, "valueChanged");
        return obj;
    }

    if (tag == "file-selector") {
        auto fileSelector = new FileSelector;
        auto obj = ObjectWrapper::wrap(fileSelector, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{
                                                                                                  "isOpenFile",
                                                                                                  "allowsMultipleFiles",
                                                                                                  "filter",
                                                                                                  "title",
                                                                                                  "selectedFilter",
                                                                                                  "files",
                                                                                              });
        OBJECT_WRAPPER_BIND_SIGNAL(fileSelector, obj, fileChanged);
        return obj;
    }

    if (tag == "form-layout")
        return ObjectWrapper::wrap(new FormLayout, jsGlobal->engine(), {
                                                                           "addRow",
                                                                           "addElement",
                                                                           "insertRow",
                                                                           "insertElement",
                                                                           "horizontalSpacing",
                                                                           "verticalSpacing",
                                                                           "rowCount"});

    if (tag == "grid-layout")
        return ObjectWrapper::wrap(new GridLayout, jsGlobal->engine(), {
                                                                           "addElement",
                                                                           "addElement",
                                                                           "rowMinimumHeight",
                                                                           "columnMinimumWidth",
                                                                           "setRowMinimumHeight",
                                                                           "setColumnMinimumWidth",
                                                                           "rowStretch",
                                                                           "columnStretch",
                                                                           "setRowStretch",
                                                                           "setColumnStretch",
                                                                           "horizontalSpacing",
                                                                           "verticalSpacing",
                                                                           "rowCount",
                                                                           "columnCount",
                                                                           "count",
                                                                           "originCorner"
                                                                       });

    if (tag == "label")
        return ObjectWrapper::wrap(new QLabel, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{
                                                                                            "text"});

    if (tag == "line-edit") {
        auto lineEdit = new QLineEdit;
        auto obj = ObjectWrapper::wrap(lineEdit, jsGlobal->engine(),
                                       qWidgetGeneralKeys + QStringList{
                                                                "acceptableInput",
                                                                "cursorPosition",
                                                                "hasSelectedText",
                                                                "inputMask",
                                                                "maxLength",
                                                                "placeholderText",
                                                                "readOnly",
                                                                "selectedText",
                                                                "text",
                                                                "clear",
                                                                "copy",
                                                                "selectAll"});
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, cursorPositionChanged);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, editingFinished);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, inputRejected);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, returnPressed);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, selectionChanged);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, textChanged);
        OBJECT_WRAPPER_BIND_SIGNAL(lineEdit, obj, textEdited);
        return obj;
    }

    if (tag == "radio") {
        auto btn = new QRadioButton;
        auto obj = ObjectWrapper::wrap(
            btn, jsGlobal->engine(), qWidgetGeneralKeys + QStringList{
                                                              "autoExclusive",
                                                              "checkable",
                                                              "checked",
                                                              "text"});
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, clicked);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, pressed);
        OBJECT_WRAPPER_BIND_SIGNAL(btn, obj, released);
        return obj;
    }

    if (tag == "select") {
        auto comboBox = new ComboBox(window());
        auto obj = ObjectWrapper::wrap(comboBox, jsGlobal->engine(),
                                       qWidgetGeneralKeys + QStringList{
                                                                "currentIndex",
                                                                "addOption",
                                                                "optionAt"});
        ObjectWrapper::bindSignal(comboBox, QOverload<int>::of(&ComboBox::currentIndexChanged), obj,
                                  "currentIndexChanged");
        return obj;
    }

    if (tag == "slider") {
        auto slider = new Slider(window());
        auto obj = ObjectWrapper::wrap(slider, jsGlobal->engine(),
                                       qWidgetGeneralKeys + QStringList{
                                                                "maximum",
                                                                "minimum",
                                                                "pageStep",
                                                                "singleStep",
                                                                "value",
                                                                "tickInterval",
                                                                "hasTicks"});
        OBJECT_WRAPPER_BIND_SIGNAL(slider, obj, valueChanged);
        return obj;
    }

    if (tag == "spin-box") {
        auto spinBox = new QSpinBox;
        auto obj = ObjectWrapper::wrap(spinBox, jsGlobal->engine(),
                                       qWidgetGeneralKeys + QStringList{
                                                                "readOnly",
                                                                "maximum",
                                                                "minimum",
                                                                "prefix",
                                                                "singleStep",
                                                                "suffix",
                                                                "value"});
        ObjectWrapper::bindSignal(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), obj, "valueChanged");
        return obj;
    }

    if (tag == "stacked-layout") {
        return ObjectWrapper::wrap(new StackedLayout, jsGlobal->engine(), {"addElement", "insertElement", "count", "currentIndex"});
    }

    JS_THROW(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
    return QJSValue::UndefinedValue;
}