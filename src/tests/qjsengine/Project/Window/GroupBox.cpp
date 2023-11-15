#include "GroupBox.h"

#include <QJSEngine>
#include <QLayout>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

GroupBox::GroupBox(QWidget *parent) : QGroupBox(parent) {
}

void GroupBox::setContent(const QJSValue &jsWidget) {
    if (!m_content.isNull()) {
        JS_THROW("Group box content is already set");
        return;
    }
    auto *widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        m_content = jsWidget;
        auto l = new QVBoxLayout;
        l->addWidget(widget);
        setLayout(l);
        return;
    }
    auto *layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        m_content = jsWidget;
        setLayout(layout);
        return;
    }
    m_content = QJSValue::NullValue;
    JS_THROW(QJSValue::TypeError, "Invalid type of group box content");
}

QJSValue GroupBox::content() const {
    return m_content;
}
QJSValue GroupBox::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() + QStringList{
                                                                             "alignment",
                                                                             "checkable",
                                                                             "checked",
                                                                             "title",
                                                                             "content",
                                                                         });
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, clicked);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, toggled);
    return obj;
}
void GroupBox::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                      const QJSValue &children,
                                                      const std::function<QJSValue(const QJSValue &)> &renderer) {
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
    if (children.hasProperty("0")) {
        setContent(renderer(children.property(0)));
    }
}
