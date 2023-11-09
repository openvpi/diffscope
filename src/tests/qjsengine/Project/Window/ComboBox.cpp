#include "ComboBox.h"

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent) {
}

void ComboBox::addOption(const QString &option) {
    addItem(option);
}

QJSValue ComboBox::optionAt(int index) {
    if (index < count())
        return itemText(index);
    else
        return QJSValue::UndefinedValue;
}
