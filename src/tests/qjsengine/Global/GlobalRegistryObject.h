#ifndef DIFFSCOPE_GLOBALREGISTRYOBJECT_H
#define DIFFSCOPE_GLOBALREGISTRYOBJECT_H

#include <QObject>
#include <QJSValue>

class GlobalObject;
class GlobalRegistryObjectPrivate;

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
    QScopedPointer<GlobalRegistryObjectPrivate> d;
};


#endif // DIFFSCOPE_GLOBALREGISTRYOBJECT_H
