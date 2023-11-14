#ifndef DIFFSCOPE_GROUPBOX_H
#define DIFFSCOPE_GROUPBOX_H

#include <QGroupBox>

#include "../../ScriptObject.h"

class GroupBox : public QGroupBox, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(int alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(QJSValue content READ content WRITE setContent)
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit GroupBox(QWidget *parent = nullptr);

    void setContent(const QJSValue &jsWidget);
    QJSValue content() const;

private:
    QJSValue m_content = QJSValue::NullValue;
};

#endif // DIFFSCOPE_GROUPBOX_H
