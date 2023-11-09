#ifndef DIFFSCOPE_SLIDER_H
#define DIFFSCOPE_SLIDER_H

#include <QSlider>
#include <QJSValue>

class ProjectWindowObject;

class Slider : public QSlider {
    Q_OBJECT
    Q_PROPERTY(bool hasTicks READ hasTicks WRITE setHasTicks)
public:
    explicit Slider(QWidget *parent = nullptr);

    bool hasTicks() const;
    void setHasTicks(bool isTicksShown);

};

#endif // DIFFSCOPE_SLIDER_H
