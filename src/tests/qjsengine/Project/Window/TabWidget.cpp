#include "TabWidget.h"

#include <QJSEngine>
#include <QLayout>

#include "../../ObjectWrapper.h"
#include "../../Global/GlobalObject.h"

QJSValue TabWidget::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() + QStringList{
                                                                             "count",
                                                                             "currentIndex",
                                                                             "tabPosition",
                                                                             "usesScrollButtons",
                                                                             "addTab",
                                                                             "insertTab",
                                                                             "setTabEnabled",
                                                                             "isTabEnabled",
                                                                             "setTabText",
                                                                             "tabText",
                                                                             "setTabToolTip",
                                                                             "tabToolTip",
                                                                             "setTabVisible",
                                                                             "isTabVisible",
                                                                         });
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, currentChanged);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, tabBarClicked);
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, tabBarDoubleClicked);
    return obj;
}

void TabWidget::configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                       const QJSValue &children,
                                                       const std::function<QJSValue(const QJSValue &)> &renderer) {
    int childrenCount = children.property("length").toInt();
    for (int i = 0; i < childrenCount; i++) {
        auto child = children.property(i);
        int index = addTab(renderer(child), child.property("attributes").property("tab-widget-label").toString());
        if (child.property("attributes").property("tab-widget-enabled").isBool())
            setTabEnabled(index, child.property("attributes").property("tab-widget-enabled").toBool());
        if (child.property("attributes").property("tab-widget-visible").isBool())
            setTabVisible(index, child.property("attributes").property("tab-widget-visible").toBool());
        if (child.property("attributes").property("tab-widget-tool-tip").isString())
            setTabToolTip(index, child.property("attributes").property("tab-widget-tool-tip").toString());
    }
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, attributes, children, renderer);
}

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
}

void TabWidget::setTabPosition(int position) {
    QTabWidget::setTabPosition(TabPosition(position));
}

int TabWidget::addTab(const QJSValue &jsWidget, const QString &label) {
    auto widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        return QTabWidget::addTab(widget, label);
    }
    auto layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        auto w = new QWidget;
        w->setLayout(layout);
        return QTabWidget::addTab(w, label);
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of tab widget content");
    return 0;
}

int TabWidget::insertTab(int index, const QJSValue &jsWidget, const QString &label) {
    auto widget = GlobalObject::getOfWrappedObject<QWidget>(jsWidget);
    if (widget) {
        return QTabWidget::insertTab(index, widget, label);
    }
    auto layout = GlobalObject::getOfWrappedObject<QLayout>(jsWidget);
    if (layout) {
        auto w = new QWidget;
        w->setLayout(layout);
        return QTabWidget::insertTab(index, w, label);
    }
    JS_THROW(QJSValue::TypeError, "Invalid type of tab widget content");
    return 0;
}

void TabWidget::setTabEnabled(int index, bool enabled) {
    QTabWidget::setTabEnabled(index, enabled);
}

bool TabWidget::isTabEnabled(int index) const {
    return QTabWidget::isTabEnabled(index);
}

void TabWidget::setTabText(int index, const QString &label) {
    QTabWidget::setTabText(index, label);
}
QString TabWidget::tabText(int index) const {
    return QTabWidget::tabText(index);
}
void TabWidget::setTabToolTip(int index, const QString &tip) {
    QTabWidget::setTabToolTip(index, tip);
}
QString TabWidget::tabToolTip(int index) const {
    return QTabWidget::tabToolTip(index);
}
void TabWidget::setTabVisible(int index, bool visible) {
    QTabWidget::setTabVisible(index, visible);
}
bool TabWidget::isTabVisible(int index) {
    return QTabWidget::isTabVisible(index);
}
