#ifndef DIFFSCOPE_DIALOG_H
#define DIFFSCOPE_DIALOG_H

#include <QObject>
#include <QJSValue>

class DialogPrivate;

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
    QScopedPointer<DialogPrivate> d;
};

#endif // DIFFSCOPE_DIALOG_H
