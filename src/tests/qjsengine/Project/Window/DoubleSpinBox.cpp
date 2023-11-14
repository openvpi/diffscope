#include "DoubleSpinBox.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue DoubleSpinBox::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() + QStringList{"readOnly", "maximum", "minimum",
                                                                                     "prefix", "singleStep", "suffix",
                                                                                     "value", "decimals"});
    ObjectWrapper::bindSignal(this, QOverload<double>::of(&QDoubleSpinBox::valueChanged), obj, "valueChanged");
    return obj;
}

DoubleSpinBox::DoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent) {
}
