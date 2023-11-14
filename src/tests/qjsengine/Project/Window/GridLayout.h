#ifndef DIFFSCOPE_GRIDLAYOUT_H
#define DIFFSCOPE_GRIDLAYOUT_H

#include <QGridLayout>

#include "../../ScriptObject.h"

class GridLayout : public QGridLayout, public ScriptDescriptiveObject {
    Q_OBJECT
    Q_PROPERTY(int originCorner READ originCorner WRITE setOriginCorner)
    Q_PROPERTY(int rowCount READ rowCount)
    Q_PROPERTY(int columnCount READ columnCount)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
    Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)
public:
    QJSValue createScriptObject() override;
    void configureThisScriptObjectByDescription(QJSValue wrappedObject, QJSValue objectIdMap, const QJSValue &attributes,
                                                const QJSValue &children,
        const std::function<QJSValue(const QJSValue &, QJSValue)> &renderer) override;

    explicit GridLayout(QWidget *parent = nullptr);
    void setOriginCorner(int corner);
    int originCorner() const;

public slots:
    void addElement(const QJSValue &jsWidget, int row, int column, int alignment = 0);
    void addElement(const QJSValue &jsWidget, int row, int column, int rowSpan, int columnSpan, int alignment = 0);

    int rowMinimumHeight(int row);
    int columnMinimumWidth(int column);
    void setRowMinimumHeight(int row, int minSize);
    void setColumnMinimumWidth(int column, int minSize);

    int rowStretch(int row);
    int columnStretch(int column);
    void setRowStretch(int row, int stretch);
    void setColumnStretch(int column, int stretch);

};

#endif // DIFFSCOPE_GRIDLAYOUT_H
