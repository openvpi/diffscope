#ifndef DIFFSCOPE_TEXTDECODER_H
#define DIFFSCOPE_TEXTDECODER_H

#include <QObject>
#include <QJSValue>
#include <QTextCodec>

class TextDecoder : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString encoding READ encoding)
    Q_PROPERTY(bool fatal READ fatal)
    Q_PROPERTY(bool ignoreBOM READ ignoreBOM)
public:
    Q_INVOKABLE TextDecoder();
    Q_INVOKABLE explicit TextDecoder(const QString &ufLabel);
    Q_INVOKABLE explicit TextDecoder(const QString &ufLabel, const QJSValue &options);

    QString encoding() const;
    bool fatal() const;
    bool ignoreBOM() const;

public slots:
    QString decode();
    QString decode(const QJSValue &buffer);
    QString decode(const QJSValue &buffer, const QJSValue &options);

private:
    QTextCodec *m_codec;
    QScopedPointer<QTextCodec::ConverterState> state;

    bool m_fatal;
    bool m_ignoreBOM;
};

#endif // DIFFSCOPE_TEXTDECODER_H
