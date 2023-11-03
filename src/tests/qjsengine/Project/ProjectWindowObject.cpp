#include "ProjectWindowObject.h"
#include "ProjectWindowObject_p.h"

#include <QApplication>
#include <QJSEngine>
#include <QMessageBox>

#include "../Global/GlobalObject.h"
#include "ProjectObject.h"

#include "Window/Dialog.h"

ProjectWindowObject::ProjectWindowObject(ProjectObject *project) : QObject(project), d(new ProjectWindowObjectPrivate{project}) {
}

ProjectWindowObject::~ProjectWindowObject() = default;

QWidget *ProjectWindowObject::window() const {
    return d->project->window();
}

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

QJSValue ProjectWindowObject::createWidget(const QString &tag) {
    if (tag == "dialog")
        return jsGlobal->engine()->newQObject(new Dialog(window()));
    jsGlobal->engine()->throwError(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
    return QJSValue::UndefinedValue;
}
