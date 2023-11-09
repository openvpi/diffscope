#ifndef DIFFSCOPE_DIALOG_H
#define DIFFSCOPE_DIALOG_H

#include <QJSValue>
#include <QDialog>

class QVBoxLayout;

class Dialog : public QDialog {
    Q_OBJECT
    Q_PROPERTY(QJSValue content READ content WRITE setContent)
public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog() override;

    void setContent(const QJSValue &jsWidget);
    QJSValue content() const;

public slots:
    bool openDialog();
    void closeDialog(bool accepted);

private:
    QVBoxLayout *m_dlgLayout;
    QJSValue m_content = QJSValue::NullValue;
};

#endif // DIFFSCOPE_DIALOG_H
