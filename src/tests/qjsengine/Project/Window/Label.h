#ifndef DIFFSCOPE_LABEL_H
#define DIFFSCOPE_LABEL_H

#include <QLabel>

#include "../../ScriptObject.h"

class Label : public QLabel, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit Label(QWidget *parent = nullptr);
};



#endif // DIFFSCOPE_LABEL_H
