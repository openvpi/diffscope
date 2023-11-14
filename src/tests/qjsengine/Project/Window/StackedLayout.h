#ifndef DIFFSCOPE_STACKEDLAYOUT_H
#define DIFFSCOPE_STACKEDLAYOUT_H

#include <QStackedLayout>

#include "../../ScriptObject.h"

class StackedLayout : public QStackedLayout, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(int count READ count)
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit StackedLayout(QWidget *parent = nullptr);

public slots:
    void addElement(const QJSValue &jsWidget);
    void insertElement(int index, const QJSValue &jsWidget);
};

#endif // DIFFSCOPE_STACKEDLAYOUT_H
