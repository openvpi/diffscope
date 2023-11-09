#ifndef DIFFSCOPE_COMBOBOX_H
#define DIFFSCOPE_COMBOBOX_H

#include <QComboBox>
#include <QJSValue>

class QComboBox;

class ProjectWindowObject;

class ComboBox : public QComboBox {
    Q_OBJECT
public:
    explicit ComboBox(QWidget *parent = nullptr);

public slots:
    void addOption(const QString &option);
    QJSValue optionAt(int index);

};

#endif // DIFFSCOPE_COMBOBOX_H
