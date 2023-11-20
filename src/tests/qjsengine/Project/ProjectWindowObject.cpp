#include "ProjectWindowObject.h"
#include <QLayout>
#include <QWidget>

#include <QDebug>

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
#include "Window/Button.h"
#include "Window/ButtonGroup.h"
#include "Window/CheckBox.h"
#include "Window/ComboBox.h"
#include "Window/Dialog.h"
#include "Window/DoubleSpinBox.h"
#include "Window/FileSelector.h"
#include "Window/FormLayout.h"
#include "Window/GridLayout.h"
#include "Window/GroupBox.h"
#include "Window/Label.h"
#include "Window/LineEdit.h"
#include "Window/Radio.h"
#include "Window/Slider.h"
#include "Window/SpinBox.h"
#include "Window/StackedLayout.h"
#include "Window/TabWidget.h"

#define ADD_ELEMENT_CLASS(Class) addElementClass<Class>(camelToHyphen(#Class))

static QString camelToHyphen(const QString &s) {
    QString ret = "";
    for (auto c : s) {
        if (c.isUpper() && !ret.isEmpty())
            ret.append('-');
        ret.append(c.toLower());
    }
    return ret;
}

ProjectWindowObject::ProjectWindowObject(ProjectObject *project) : QObject(project), m_project(project) {
    ADD_ELEMENT_CLASS(BoxLayout);
    ADD_ELEMENT_CLASS(Button);
    ADD_ELEMENT_CLASS(CheckBox);
    ADD_ELEMENT_CLASS(ComboBox);
    ADD_ELEMENT_CLASS(DoubleSpinBox);
    ADD_ELEMENT_CLASS(FileSelector);
    ADD_ELEMENT_CLASS(FormLayout);
    ADD_ELEMENT_CLASS(GridLayout);
    ADD_ELEMENT_CLASS(GroupBox);
    ADD_ELEMENT_CLASS(Label);
    ADD_ELEMENT_CLASS(LineEdit);
    ADD_ELEMENT_CLASS(Radio);
    ADD_ELEMENT_CLASS(Slider);
    ADD_ELEMENT_CLASS(SpinBox);
    ADD_ELEMENT_CLASS(StackedLayout);
    ADD_ELEMENT_CLASS(TabWidget);

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
    jsGlobal->defineEnum("TabPosition", {
                                            {"North"},
                                            {"South"},
                                            {"West"},
                                            {"East"},
                                        });
}

ProjectWindowObject::~ProjectWindowObject() = default;

QWidget *ProjectWindowObject::window() const {
    return m_project->win();
}

void ProjectWindowObject::finalizeScriptScope() {
    for(auto obj: m_scriptScopedObjects) {
        obj->deleteLater();
    }
    m_scriptScopedIdSpecifiedElements.clear();
}

void ProjectWindowObject::alert(const QString &message, const QString &title) {
    QMessageBox::information(m_project->win(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

void ProjectWindowObject::warning(const QString &message, const QString &title) {
    QMessageBox::warning(m_project->win(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

void ProjectWindowObject::critical(const QString &message, const QString &title) {
    QMessageBox::critical(m_project->win(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

bool ProjectWindowObject::question(const QString &message, const QString &title) {
    return QMessageBox::question(m_project->win(), title.isEmpty() ? qApp->applicationDisplayName() : title,
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

    if (m_elementDict.contains(tag)) {
        return m_elementDict.value(tag)();
    }

    JS_THROW(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
    return {};
}

QJSValue ProjectWindowObject::renderElement(const QJSValue &description) {
    auto targetDescription = description;
    if (description.isString()) {
        QDomDocument doc;
        doc.setContent(description.toString());
        if (!doc.documentElement().isNull())
            targetDescription = convertXmlToDescription(doc.documentElement());
    }
    auto tag = targetDescription.property("tag").toString();
    if (!m_elementDescriptiveDict.contains(tag)) {
        JS_THROW(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
        return {};
    }
    auto elm = m_elementDescriptiveDict.value(tag)(targetDescription.property("attributes"), targetDescription.property("children"));
    return elm;
}

QJSValue ProjectWindowObject::convertXmlToDescription(const QDomNode &node) {
    QList<QJSValue> children;

    QDomNode childNode = node.firstChild();
    while (!childNode.isNull()) {
        if (childNode.isElement() || childNode.isText()) {
            auto child = convertXmlToDescription(childNode);
            children.append(child);
        }
        childNode = childNode.nextSibling();
    }

    if (node.isElement()) {
        auto elementJson = jsGlobal->engine()->newObject();;
        elementJson.setProperty("tag", camelToHyphen(node.toElement().tagName()));

        auto attributes = node.attributes();
        auto attributesJson = jsGlobal->engine()->newObject();
        for (int i = 0; i < attributes.count(); ++i) {
            QDomAttr attribute = attributes.item(i).toAttr();
            attributesJson.setProperty(attribute.name(), attribute.value());
        }
        elementJson.setProperty("attributes", attributesJson);
        elementJson.setProperty("children", jsGlobal->engine()->toScriptValue(children));
        return elementJson;
    } else {
        return node.nodeValue();
    }
}

QJSValue ProjectWindowObject::getElementById(const QString &id) const {
    return m_scriptScopedIdSpecifiedElements.value(id, QJSValue::NullValue);
}

void ProjectWindowObject::bindIdAccessorToElement(QJSValue element) {
    ObjectWrapper::addAccessorProperty(element, jsGlobal->engine(), "id", [=]() {
        return element.property("_elmId");
    }, [=](const QJSValue &id) {
        if (element.property("_elmId").isString())
            m_scriptScopedIdSpecifiedElements.remove(element.property("_elmId").toString());
        if (id.isString())
            m_scriptScopedIdSpecifiedElements.insert(id.toString(), element);
    });
    element.setProperty("_elmId", QJSValue::NullValue);
}
