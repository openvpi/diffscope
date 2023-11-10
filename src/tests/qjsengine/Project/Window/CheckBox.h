#ifndef DIFFSCOPE_CHECKBOX_H
#define DIFFSCOPE_CHECKBOX_H

#include <QCheckBox>

class CheckBox : public QCheckBox {
    Q_OBJECT
    Q_PROPERTY(int checkState READ checkState WRITE setCheckState)
public:
    explicit CheckBox(QWidget *parent = nullptr);
    void setCheckState(int state);
    int checkState() const;
};



#endif // DIFFSCOPE_CHECKBOX_H
