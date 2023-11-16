#ifndef DIFFSCOPE_TEXTENCODER_H
#define DIFFSCOPE_TEXTENCODER_H

#include <QObject>
#include <QJSValue>

class TextEncoder : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString encoding READ encoding)
public:
    Q_INVOKABLE TextEncoder();

    static QString encoding() {
        return "utf-8";
    }

public slots:
    QJSValue encode(const QString &string);

};

#endif // DIFFSCOPE_TEXTENCODER_H
