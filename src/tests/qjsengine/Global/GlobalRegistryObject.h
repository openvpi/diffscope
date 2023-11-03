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
    QJSValue scriptConstructor(const QString &id) const;

    void clearRegistry();

public slots:
    void registerScript(const QJSValue &scriptConstructor);

private:
    QHash<QString, QJSValue> m_scriptDict;
};


#endif // DIFFSCOPE_GLOBALREGISTRYOBJECT_H
