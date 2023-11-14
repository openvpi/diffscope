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

public slots:
    void alert(const QString &message, const QString &title = {});
    void warning(const QString &message, const QString &title = {});
    void critical(const QString &message, const QString &title = {});
    bool question(const QString &message, const QString &title = {});

    QJSValue createButtonGroup();
    QJSValue createDialog();
    QJSValue createElement(const QString &tag);
    QJSValue renderElement(const QJSValue &description, QJSValue objectIdMap);

private:
    ProjectObject *m_project;
    QMap<QString, std::function<QJSValue ()>> m_elementDict;
    QMap<QString, std::function<QJSValue (QJSValue, const QJSValue &, const QJSValue &)>> m_elementDescriptiveDict;

    template<class T>
    void addElementClass(const QString &tagName) {
        m_elementDict.insert(tagName, []() {
            auto t = new T;
            return t->createScriptObject();
        });
        m_elementDescriptiveDict.insert(tagName, [this](QJSValue objectIdMap, const QJSValue &attributes, const QJSValue &children) {
            auto t = new T;
            auto obj = t->createScriptObject();
            t->configureThisScriptObjectByDescription(obj, objectIdMap, attributes, children,
                                                      [this](const QJSValue &arg1, QJSValue arg2) { return renderElement(arg1, arg2); });
            return obj;
        });
    }
};

#endif // DIFFSCOPE_PROJECTWINDOWOBJECT_H
