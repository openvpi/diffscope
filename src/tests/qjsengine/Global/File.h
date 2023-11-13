#ifndef DIFFSCOPE_FILE_H
#define DIFFSCOPE_FILE_H

#include <QJSValue>
#include <QObject>

class QFile;

class File : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QJSValue lastModifiedDate READ lastModifiedDate)
    Q_PROPERTY(int size READ size WRITE resize)
    Q_PROPERTY(int pos READ pos WRITE seek)
    Q_PROPERTY(int openMode READ openMode)
public:
    explicit File(const QString &path);
    ~File() override;

    QString name() const;
    QJSValue lastModifiedDate() const;

    int size() const;
    void resize(int newSize);
    int pos() const;
    void seek(int pos);

    int openMode() const;

public slots:
    void open(int openMode);
    void close();

    QJSValue read(int readSize);
    QJSValue readAll();
    QString readAllAsUtf8();
    int write(const QJSValue &data);
    void flush();

private:
    QFile *f;
};



#endif // DIFFSCOPE_FILE_H
