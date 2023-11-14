#include "Slider.h"

#include "../../Global/GlobalObject.h"
#include "../../ObjectWrapper.h"
#include <QJSEngine>

QJSValue Slider::createScriptObject() {
    auto obj = ObjectWrapper::wrap(this, jsGlobal->engine(),
                                   ObjectWrapper::qWidgetGeneralKeys() + QStringList{"maximum", "minimum", "pageStep",
                                                                                     "singleStep", "value",
                                                                                     "tickInterval", "hasTicks"});
    OBJECT_WRAPPER_BIND_SIGNAL(this, obj, valueChanged);
    return obj;
}

Slider::Slider(QWidget *parent) : QSlider(parent) {
    setOrientation(Qt::Horizontal);
}

bool Slider::hasTicks() const {
    return tickPosition() != NoTicks;
}

void Slider::setHasTicks(bool isTicksShown) {
    setTickPosition(isTicksShown ? TicksBelow : NoTicks);
}
