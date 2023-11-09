#ifndef DIFFSCOPE_FORMLAYOUT_H
#define DIFFSCOPE_FORMLAYOUT_H

#include <QFormLayout>
#include <QJSValue>

class FormLayout : public QFormLayout {
    Q_OBJECT
public:
    explicit FormLayout(QWidget *parent = nullptr);

public slots:
    void addRow(const QString &label, const QJSValue &jsWidget);
    void addElement(const QJSValue &jsWidget);

};

#endif // DIFFSCOPE_FORMLAYOUT_H
