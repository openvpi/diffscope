#ifndef DIFFSCOPE_FORMLAYOUT_H
#define DIFFSCOPE_FORMLAYOUT_H

#include <QObject>
#include <QJSValue>

class QFormLayout;

class FormLayout : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue _p READ layoutObject)
public:
    explicit FormLayout(QWidget *win);

    QJSValue layoutObject() const {
        return m_layoutObject;
    }

public slots:
    void addRow(const QString &label, const QJSValue &jsWidget);
    void addElement(const QJSValue &jsWidget);

private:
    QFormLayout *m_layout;
    QJSValue m_layoutObject;

};

#endif // DIFFSCOPE_FORMLAYOUT_H
