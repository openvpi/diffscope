#ifndef DIFFSCOPE_STACKEDLAYOUT_H
#define DIFFSCOPE_STACKEDLAYOUT_H

#include <QStackedLayout>
#include <QJSValue>

class StackedLayout : public QStackedLayout {
    Q_OBJECT
    Q_PROPERTY(int count READ count)
public:
    explicit StackedLayout(QWidget *parent = nullptr);

public slots:
    void addElement(const QJSValue &jsWidget);
    void insertElement(int index, const QJSValue &jsWidget);
};

#endif // DIFFSCOPE_STACKEDLAYOUT_H
