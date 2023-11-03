#ifndef DIFFSCOPE_PROJECTWINDOWOBJECT_H
#define DIFFSCOPE_PROJECTWINDOWOBJECT_H

#include <QJSValue>
#include <QObject>

class ProjectObject;

class ProjectWindowObject : public QObject {
    Q_OBJECT
public:
    explicit ProjectWindowObject(ProjectObject *project);
    ~ProjectWindowObject() override;

    QWidget *window() const;

public slots:
    void alert(const QString &message, const QString &title = {});
    void warning(const QString &message, const QString &title = {});
    void critical(const QString &message, const QString &title = {});
    bool question(const QString &message, const QString &title = {});

    QJSValue createElement(const QString &tag);

private:
    ProjectObject *m_project;
};

#endif // DIFFSCOPE_PROJECTWINDOWOBJECT_H
