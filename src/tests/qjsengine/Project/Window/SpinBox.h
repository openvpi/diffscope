#ifndef DIFFSCOPE_SPINBOX_H
#define DIFFSCOPE_SPINBOX_H

#include <QSpinBox>

#include "../../ScriptObject.h"

class SpinBox : public QSpinBox, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    QJSValue createScriptObject() override;
    explicit SpinBox(QWidget *parent = nullptr);
};



#endif // DIFFSCOPE_SPINBOX_H
