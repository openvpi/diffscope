#ifndef DIFFSCOPE_FORMLAYOUT_H
#define DIFFSCOPE_FORMLAYOUT_H

#include <QFormLayout>

#include "../../ScriptObject.h"

class FormLayout : public QFormLayout, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount)
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit FormLayout(QWidget *parent = nullptr);

public slots:
    void addRow(const QString &label, const QJSValue &jsWidget);
    void addElement(const QJSValue &jsWidget);

    void insertRow(int row, const QString &label, const QJSValue &jsWidget);
    void insertElement(int row, const QJSValue &jsWidget);

};

#endif // DIFFSCOPE_FORMLAYOUT_H
