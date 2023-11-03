#ifndef DIFFSCOPE_DIALOG_H
#define DIFFSCOPE_DIALOG_H

#include <QJSValue>
#include <QObject>

class QDialog;
class QVBoxLayout;

class Dialog : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue content READ content WRITE setContent)
public:
    explicit Dialog(QWidget *win);
    ~Dialog() override;

    void setContent(const QJSValue &jsWidget);
    QJSValue content() const;

public slots:
    bool open();
    void close(bool accepted);

private:
    QDialog *m_dlg;
    QVBoxLayout *m_dlgLayout;
    QJSValue m_content = QJSValue::NullValue;
};

#endif // DIFFSCOPE_DIALOG_H
