#ifndef DIFFSCOPE_GLOBALOBJECT_H
#define DIFFSCOPE_GLOBALOBJECT_H

#include <QJSValue>
#include <QObject>

class QJSEngine;

class GlobalRegistryObject;
class GlobalStorageObject;
class Console;

class GlobalObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QJSValue registry READ jsRegistry)
    Q_PROPERTY(QJSValue storage READ jsStorage)
public:
    explicit GlobalObject(QObject *parent = nullptr);
    ~GlobalObject() override;
    static GlobalObject *instance();

    //========Helper functions========//
    QJSEngine *engine() const;
    QJSValue load(const QString &scriptFilename);
    struct JSEnumEntry {
        QString s;
        int i = -1;
    };
    void defineEnum(const QString &enumName, const QList<JSEnumEntry> &entries);
    QString stackTrace(int depth = 0);
    QString fileTrace(int depth = 0);

    //========Objects (C++)========//
    GlobalRegistryObject *registry() const;
    GlobalStorageObject *storage() const;
    Console *console() const;

    //========Objects (JavaScript)========//
    QJSValue jsRegistry() const;
    QJSValue jsStorage() const;

public slots:
    void pause();

private:
    QJSEngine *m_engine;

    GlobalRegistryObject *m_registry;
    GlobalStorageObject *m_storage;
    Console *m_console;

    QJSValue m_registryObject;
    QJSValue m_storageObject;

    void installConsole();
    void installTextCodec();
    void initializeJavaScriptGlobal();
};

#define jsGlobal GlobalObject::instance()
#define JS_QOBJ jsGlobal->engine()->newQObject
#define JS_THROW jsGlobal->engine()->throwError

#endif // DIFFSCOPE_GLOBALOBJECT_H
