#include "SpinBox.h"

#include <QJSEngine>

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"

QJSValue SpinBox::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() + QStringList{
                                                            "readOnly",
                                                            "maximum",
                                                            "minimum",
                                                            "prefix",
                                                            "singleStep",
                                                            "suffix",
                                                            "value"});
    ObjectWrapper::bindSignal(this, QOverload<int>::of(&QSpinBox::valueChanged), obj, "valueChanged");
    return obj;
}

SpinBox::SpinBox(QWidget *parent) : QSpinBox(parent) {
}
