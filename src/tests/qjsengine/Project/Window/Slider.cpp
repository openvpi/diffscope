#include "Slider.h"

#include <QJSEngine>


Slider::Slider(QWidget *parent) : QSlider(parent) {
    setOrientation(Qt::Horizontal);
}

bool Slider::hasTicks() const {
    return tickPosition() != NoTicks;
}

void Slider::setHasTicks(bool isTicksShown) {
    setTickPosition(isTicksShown ? TicksBelow : NoTicks);
}
