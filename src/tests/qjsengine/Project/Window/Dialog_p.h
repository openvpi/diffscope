#ifndef DIFFSCOPE_DIALOG_P_H
#define DIFFSCOPE_DIALOG_P_H

#include "Dialog.h"

#include <QDialog>
#include <QVBoxLayout>

class DialogPrivate {
public:
    QDialog *dlg;
    QVBoxLayout *dlgLayout;
    QJSValue content = QJSValue::NullValue;
};

#endif // DIFFSCOPE_DIALOG_P_H
