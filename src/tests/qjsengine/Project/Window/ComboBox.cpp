#include "ComboBox.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue ComboBox::createScriptObject() {
    auto obj =
        ObjectWrapper::wrap(this, jsGlobal->engine(),
                            ObjectWrapper::qWidgetGeneralKeys() + QStringList{"currentIndex", "addOption", "optionAt"});
    ObjectWrapper::bindSignal(this, QOverload<int>::of(&ComboBox::currentIndexChanged), obj, "currentIndexChanged");
    return obj;
}
void ComboBox::configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                      const QJSValue &children,
    const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) {
    int childrenCount = children.property("length").toInt();
    for (int i = 0; i < childrenCount; i++) {
        auto child = children.property(i);
        if (child.property("tag").toString() == "combo-box-option") {
            addOption(child.property("children").property(0).toString());
        }
    }
    ScriptDescriptiveObject::configureThisScriptObjectByDescription(wrappedObject, objectIdMap, attributes, children,
                                                                    renderer);
}

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent) {
}

void ComboBox::addOption(const QString &option) {
    addItem(option);
}

void ComboBox::insertOption(int index, const QString &option) {
    insertItem(index, option);
}

void ComboBox::removeOption(int index) {
    removeItem(index);
}

void ComboBox::clear() {
    QComboBox::clear();
}

QJSValue ComboBox::optionAt(int index) const {
    if (index < count())
        return itemText(index);
    else
        return QJSValue::UndefinedValue;
}
