#include "ProjectWindowObject.h"
#include "ProjectWindowObject_p.h"

#include <QApplication>
#include <QJSEngine>
#include <QMessageBox>

#include "ProjectObject.h"

ProjectWindowObject::ProjectWindowObject(ProjectObject *project) : QObject(project), d(new ProjectWindowObjectPrivate{project}) {
}

ProjectWindowObject::~ProjectWindowObject() = default;

void ProjectWindowObject::alert(const QString &message, const QString &title) {
    QMessageBox::information(d->project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

void ProjectWindowObject::warning(const QString &message, const QString &title) {
    QMessageBox::warning(d->project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

void ProjectWindowObject::critical(const QString &message, const QString &title) {
    QMessageBox::critical(d->project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message);
}

bool ProjectWindowObject::question(const QString &message, const QString &title) {
    return QMessageBox::question(d->project->window(), title.isEmpty() ? qApp->applicationDisplayName() : title, message) == QMessageBox::Yes;
}

QJSValue ProjectWindowObject::createDialog() {
    return QJSValue();
}

QJSValue ProjectWindowObject::createWidget(const QString &tag) {
    return QJSValue();
}
