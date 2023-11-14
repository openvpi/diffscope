#ifndef DIFFSCOPE_DOUBLESPINBOX_H
#define DIFFSCOPE_DOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "../../ScriptObject.h"

class DoubleSpinBox : public QDoubleSpinBox, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    QJSValue createScriptObject() override;

    explicit DoubleSpinBox(QWidget *parent = nullptr);
};



#endif // DIFFSCOPE_DOUBLESPINBOX_H
