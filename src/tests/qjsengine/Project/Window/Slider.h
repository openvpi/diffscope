#ifndef DIFFSCOPE_SLIDER_H
#define DIFFSCOPE_SLIDER_H

#include <QSlider>

#include "../../ScriptObject.h"

class ProjectWindowObject;

class Slider : public QSlider, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(bool hasTicks READ hasTicks WRITE setHasTicks)
public:
    QJSValue createScriptObject() override;
    explicit Slider(QWidget *parent = nullptr);

    bool hasTicks() const;
    void setHasTicks(bool isTicksShown);

};

#endif // DIFFSCOPE_SLIDER_H
