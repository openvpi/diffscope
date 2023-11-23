#ifndef DIFFSCOPE_JAVASCRIPTOUTPUTWIDGET_H
#define DIFFSCOPE_JAVASCRIPTOUTPUTWIDGET_H

#include <QWidget>

class QTextBrowser;

class JavaScriptOutputWidget : public QWidget {
    Q_OBJECT;
public:
    explicit JavaScriptOutputWidget(QWidget *parent = nullptr);

public slots:
    void receiveMessage(int level, const QString &message, const QString &fileTrace);
    void clear();

private:
    QTextBrowser *m_textEdit;
    int m_levelFlags = -1;
    void setLevelFlag(int flag, bool enabled);

    bool m_showFileTrace = true;
    bool m_showTime = false;
    QString m_cachedMessages;

};

#endif // DIFFSCOPE_JAVASCRIPTOUTPUTWIDGET_H
