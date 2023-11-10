#ifndef DIFFSCOPE_GRIDLAYOUT_H
#define DIFFSCOPE_GRIDLAYOUT_H

#include <QGridLayout>
#include <QJSValue>

class GridLayout : public QGridLayout {
    Q_OBJECT
    Q_PROPERTY(int originCorner READ originCorner WRITE setOriginCorner)
    Q_PROPERTY(int rowCount READ rowCount)
    Q_PROPERTY(int columnCount READ columnCount)
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
    Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)
public:
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
