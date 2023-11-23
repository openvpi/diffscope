#ifndef DIFFSCOPE_GLOBALREGISTRYOBJECT_H
#define DIFFSCOPE_GLOBALREGISTRYOBJECT_H

#include <QHash>
#include <QJSValue>
#include <QObject>

class GlobalObject;

class GlobalRegistryObject : public QObject {
    Q_OBJECT
public:
    explicit GlobalRegistryObject(GlobalObject *global);
    ~GlobalRegistryObject() override;

    QStringList scripts() const;
    struct ScriptSpec {
        bool isBuiltIn = false;
        bool isVisibleToUser = true;
        QJSValue scriptConstructor;
    };
    ScriptSpec scriptSpec(const QString &id) const;
    QJSValue scriptConstructor(const QString &id) const;
    void loadScript(const QString &userScriptDir);
    void clearRegistry();

    template <class T>
    void registerScriptImplementation(bool isVisibleToUser = true) {
        registerScriptImplementationImpl(&T::staticMetaObject, T::manifest(), isVisibleToUser);
    }

public slots:
    void registerScript(const QJSValue &scriptConstructor);

private:
    friend class GlobalObject;
    ScriptSpec m_currentScriptSpec;
    QHash<QString, ScriptSpec> m_scriptDict;

    QJSValue m_builtInScriptHelper;
    void registerScriptImplementationImpl(const QMetaObject *impl, const QJSValue &manifest, bool isVisibleToUser);
    void registerBuiltInScripts();
};


#endif // DIFFSCOPE_GLOBALREGISTRYOBJECT_H
