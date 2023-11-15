#ifndef DIFFSCOPE_BUTTON_H
#define DIFFSCOPE_BUTTON_H

#include <QPushButton>
#include "../../ScriptObject.h"

class Button : public QPushButton, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    explicit Button(QWidget *parent = nullptr);

    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                const QJSValue &children,
                                                const std::function<QJSValue(const QJSValue &)> &renderer) override;
};



#endif // DIFFSCOPE_BUTTON_H
