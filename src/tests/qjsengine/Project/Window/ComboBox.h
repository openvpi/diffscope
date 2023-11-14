#ifndef DIFFSCOPE_COMBOBOX_H
#define DIFFSCOPE_COMBOBOX_H

#include <QComboBox>

#include "../../ScriptObject.h"

class QComboBox;

class ProjectWindowObject;

class ComboBox : public QComboBox, public ScriptDescriptiveObject {
    Q_OBJECT
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;
    explicit ComboBox(QWidget *parent = nullptr);

public slots:
    void addOption(const QString &option);
    void insertOption(int index, const QString &option);
    void removeOption(int index);
    void clear();

    QJSValue optionAt(int index) const;

};

#endif // DIFFSCOPE_COMBOBOX_H
