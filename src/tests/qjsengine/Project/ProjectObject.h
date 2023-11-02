#ifndef DIFFSCOPE_PROJECTOBJECT_H
#define DIFFSCOPE_PROJECTOBJECT_H

#include <QObject>
#include <QJSValue>

class QWidget;
class QJSEngine;

class ProjectObjectPrivate;

class ProjectObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue window READ jsWindow)
public:
    explicit ProjectObject(QWidget *window);
    ~ProjectObject() override;

    QWidget *window() const;
    QJSValue invoke(const QString &id, int index = -1);

    QJSValue jsWindow() const;

private:
    QScopedPointer<ProjectObjectPrivate> d;
};


#endif // DIFFSCOPE_PROJECTOBJECT_H
