#include "Console.h"

#include <QDateTime>
#include <QDebug>
#include <QJSEngine>

#include "GlobalObject.h"

Console::Console(QObject *parent) : QObject(parent) {
    connect(this, &Console::messagePrinted, [=](int level, const QString &message, const QString &fileTrace) {
        QString prifix;
        switch (Level(level)) {
          case Debug:
              prifix = QString("[js: debug][%1] ").arg(fileTrace);
              break;
          case Info:
              prifix = QString("[js: info][%1] ").arg(fileTrace);
              break;
          case Log:
              prifix = QString("[js: log][%1] ").arg(fileTrace);
              break;
          case Warning:
              prifix = QString("[js: warning][%1] ").arg(fileTrace);
              break;
          case Error:
              prifix = QString("[js: error][%1] ").arg(fileTrace);
              break;
        }
        for (const auto &s: message.split("\n"))
          qDebug().noquote().nospace() << prifix << s;
    });
}

QString Console::concatenateMessage(const QJSValue &values) {
    if (values.property("length").strictlyEquals(0))
        return "";
    auto formatStr = values.property(0).toString();
    int index = 1;
    QRegExp rx(R"(%(\d*)(.?)(\d*)([dfioOs]))", Qt::CaseSensitive);
    int pos = 0;
    while ((pos = rx.indexIn(formatStr, pos)) != -1) {
        if (index >= values.property("length").toInt())
            break;
        auto cap1 = rx.cap(1).toInt();
        auto cap2 = rx.cap(2).isEmpty();
        bool hasCap3;
        auto cap3 = rx.cap(3).toInt(&hasCap3);
        auto flag = rx.cap(4);

        int length = rx.matchedLength();
        if (flag == "s" || flag == "o" || flag == "O") {
            length = values.property(index).toString().length();
            formatStr.replace(pos, rx.matchedLength(), values.property(index).toString());
            index++;
        } else if (flag == "d" || flag == "i") {
            auto fillWidth = cap2 ? cap1 : cap3;
            auto fillWithZero = !cap2;
            QString s = QString("%1").arg(values.property(index).toInt(), fillWidth, 10, fillWithZero ? QChar('0') : QChar(' '));
            length = s.length();
            formatStr.replace(pos, rx.matchedLength(), s);
            index++;
        } else if (flag == "f") {
            auto fillWidth = cap1;
            auto precision = cap3;
            QString s = QString("%1").arg(values.property(index).toNumber(), fillWidth, 'f', hasCap3 ? precision : -1);
            length = s.length();
            formatStr.replace(pos, rx.matchedLength(), s);
            index++;
        }
        pos += length;
    }
    for(; index < values.property("length").toInt(); index++) {
        formatStr += (" " + values.property(index).toString());
    }
    return formatStr;
}

void Console::printUncaughtError(const QJSValue &error) {
    QString message;
    if (error.isError()) {
        message = QString("Uncaught %1: %2")
                      .arg(error.property("name").toString(), error.property("message").toString());
        for(const auto &s: error.property("stack").toString().split("\n"))
            message += ("\n\t" + s);
    } else {
        message = "Uncaught " + message;
    }

    emit print(Error, message, error.property("fileName").toString());
}

void Console::print(Console::Level level, const QString &message, const QString &fileTrace) {
    emit messagePrinted(level, message, fileTrace.isEmpty() ? jsGlobal->fileTrace(1) : fileTrace);
}

void Console::assert(bool assertion, const QJSValue &args) {
    if (!assertion)
        print(Error, "Assertion failed: " + concatenateMessage(args));
}

void Console::clear() {
    emit clearRequested();
}

void Console::count(const QJSValue &label) {
    QString counterName = label.isUndefined() ? "default" : label.toString();
    print(Log, counterName + ": " + QString::number(++m_counters[counterName]));
}

void Console::countReset(const QJSValue &label) {
    QString counterName = label.isUndefined() ? "default" : label.toString();
    if (m_counters.contains(counterName)) {
        print(Log, counterName + ": 0");
        m_counters.remove(counterName);
    } else {
        print(Warning, QString("Counter '%1' does not exist.").arg(counterName));
    }

}

void Console::debug(const QJSValue &args) {
    print(Debug, concatenateMessage(args));
}

void Console::error(const QJSValue &args) {
    print(Error, concatenateMessage(args));
}

void Console::info(const QJSValue &args) {
    print(Info, concatenateMessage(args));
}

void Console::log(const QJSValue &args) {
    print(Log, concatenateMessage(args));
}

void Console::time(const QJSValue &label) {
    QString timerName = label.isUndefined() ? "default" : label.toString();
    m_timers[timerName] = QDateTime::currentMSecsSinceEpoch();
}

void Console::timeEnd(const QJSValue &label) {
    QString timerName = label.isUndefined() ? "default" : label.toString();
    if (m_timers.contains(timerName)) {
        print(Log, timerName + ": " + QString::number(QDateTime::currentMSecsSinceEpoch() - m_timers[timerName]) + "ms");
        m_timers.remove(timerName);
    } else {
        print(Warning, QString("Counter '%1' does not exist.").arg(timerName));
    }
}

void Console::timeLog(const QJSValue &label, const QJSValue &args) {
    QString timerName = label.isUndefined() ? "default" : label.toString();
    if (m_timers.contains(timerName)) {
        print(Log, timerName + ": " + QString::number(QDateTime::currentMSecsSinceEpoch() - m_timers[timerName]) + "ms " + concatenateMessage(args));
    } else {
        print(Warning, QString("Counter '%1' does not exist.").arg(timerName));
    }
}

void Console::trace(const QJSValue &args) {
    QString message = "Trace: " + concatenateMessage(args);
    for(const auto &s: jsGlobal->stackTrace(1).split("\n")) {
        message += ("\n\t" + s);
    }
    print(Log, message);

}

void Console::warn(const QJSValue &args) {
    print(Warning, concatenateMessage(args));
}
