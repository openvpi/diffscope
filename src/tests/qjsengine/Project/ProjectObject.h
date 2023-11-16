#ifndef DIFFSCOPE_PROJECTOBJECT_H
#define DIFFSCOPE_PROJECTOBJECT_H

#include <QHash>
#include <QJSValue>
#include <QObject>

class QWidget;
class QJSEngine;

class ProjectWindowObject;

class ProjectObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue window READ jsWindow)
public:
    explicit ProjectObject(QWidget *window);
    ~ProjectObject() override;

    QWidget *window() const;
    QJSValue invoke(const QString &id, int index = -1);

    QJSValue jsWindow() const;

public slots:
    void startProgress(const QString &title, int maximum);
    void updateProgress(int value);
    void finishProgress();

signals:
    void progressStarted(const QString &title, int maximum);
    void progressValueChanged(int value);
    void progressFinished();

private:
    QWidget *m_win;
    ProjectWindowObject *m_windowQObject;

    QHash<QString, QJSValue> m_scriptInstanceDict;

    QJSValue m_thisObject;
    QJSValue m_windowObject;

    QJSValue invokeImpl(const QString &id, int index = -1);

    bool m_isProgressStarted = false;
};


#endif // DIFFSCOPE_PROJECTOBJECT_H
