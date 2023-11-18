#ifndef DIFFSCOPE_CONSOLE_H
#define DIFFSCOPE_CONSOLE_H

#include <QObject>
#include <QJSValue>
#include <QMap>

#undef assert

class Console : public QObject {
    Q_OBJECT
public:
    explicit Console(QObject *parent = nullptr);
    enum Level {
        Debug,
        Info,
        Log,
        Warning,
        Error,
    };

    void printUncaughtError(const QJSValue &error);
    void print(Level level, const QString &message, const QString &fileTrace = {});

public slots:
    void assert(bool assertion, const QJSValue &args);
    void clear();
    void count(const QJSValue &label);
    void countReset(const QJSValue &label);
    void debug(const QJSValue &args);
    void error(const QJSValue &args);
    void info(const QJSValue &args);
    void log(const QJSValue &args);
    void time(const QJSValue &label);
    void timeEnd(const QJSValue &label);
    void timeLog(const QJSValue &label, const QJSValue &args);
    void trace(const QJSValue &args);
    void warn(const QJSValue &args);

signals:
    void messagePrinted(int level, const QString &message, const QString &fileTrace);
    void clearRequested();

private:
    QMap<QString, int> m_counters;
    static QString concatenateMessage(const QJSValue &values);
    QMap<QString, quint64> m_timers;
};

#endif // DIFFSCOPE_CONSOLE_H
