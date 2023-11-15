#ifndef DIFFSCOPE_PROJECTWINDOWOBJECT_H
#define DIFFSCOPE_PROJECTWINDOWOBJECT_H

#include <QJSValue>
#include <QJSEngine>
#include <QLayout>
#include <QObject>
#include <QMap>

#include "../Global/GlobalObject.h"
#include "../ScriptObject.h"

class ProjectObject;

class ProjectWindowObject : public QObject {
    Q_OBJECT

public:
    explicit ProjectWindowObject(ProjectObject *project);
    ~ProjectWindowObject() override;

    QWidget *window() const;

    void finalizeScriptScope();

public slots:
    void alert(const QString &message, const QString &title = {});
    void warning(const QString &message, const QString &title = {});
    void critical(const QString &message, const QString &title = {});
    bool question(const QString &message, const QString &title = {});

    QJSValue createButtonGroup();
    QJSValue createDialog();
    QJSValue createElement(const QString &tag);
    QJSValue renderElement(const QJSValue &description);
    QJSValue getElementById(const QString &id) const;

private:
    ProjectObject *m_project;
    QMap<QString, std::function<QJSValue ()>> m_elementDict;
    QMap<QString, std::function<QJSValue (const QJSValue &, const QJSValue &)>> m_elementDescriptiveDict;

    QList<QObject *> m_scriptScopedObjects;
    QMap<QString, QJSValue> m_scriptScopedIdSpecifiedElements;

    void bindIdAccessorToElement(QJSValue element);

    template<class T>
    void addElementClass(const QString &tagName) {
        m_elementDict.insert(tagName, [this]() {
            auto t = new T;
            auto obj = t->createScriptObject();
            bindIdAccessorToElement(obj);
            return obj;
        });
        m_elementDescriptiveDict.insert(tagName, [this](const QJSValue &attributes, const QJSValue &children) {
            auto t = new T;
            auto obj = t->createScriptObject();
            bindIdAccessorToElement(obj);
            t->configureThisScriptObjectByDescription(
                obj, attributes, children, [this](const QJSValue &arg) { return renderElement(arg); });
            return obj;
        });
    }
};

#endif // DIFFSCOPE_PROJECTWINDOWOBJECT_H
