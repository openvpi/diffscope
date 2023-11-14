#ifndef DIFFSCOPE_LINEEDIT_H
#define DIFFSCOPE_LINEEDIT_H

#include <QLineEdit>

#include "../../ScriptObject.h"

class LineEdit : public QLineEdit, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit LineEdit(QWidget *parent = nullptr);
};

#endif // DIFFSCOPE_LINEEDIT_H
