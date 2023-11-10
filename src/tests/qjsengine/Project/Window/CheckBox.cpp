#include "CheckBox.h"

CheckBox::CheckBox(QWidget *parent) : QCheckBox(parent) {
}

void CheckBox::setCheckState(int state) {
    QCheckBox::setCheckState(Qt::CheckState(state));
}

int CheckBox::checkState() const {
    return QCheckBox::checkState();
}
