#ifndef DIFFSCOPE_RADIO_H
#define DIFFSCOPE_RADIO_H

#include <QRadioButton>

#include "../../ScriptObject.h"

class Radio : public QRadioButton, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit Radio(QWidget *parent = nullptr);
};



#endif // DIFFSCOPE_RADIO_H
