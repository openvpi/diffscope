#ifndef DIFFSCOPE_OBJECTWRAPPER_H
#define DIFFSCOPE_OBJECTWRAPPER_H

#include <QJSValue>
#include <QObject>
#include <QVariant>

class QJSEngine;

class JSValueAccessorDescriptor : public QObject {
    Q_OBJECT;

public:
    explicit JSValueAccessorDescriptor(QJSValue obj, QString key)
        : QObject(nullptr), m_obj(std::move(obj)), m_key(std::move(key)) {
    }

public slots:
    QJSValue get() const {
        return m_obj.property(m_key);
    }
    void set(const QJSValue &value) {
        return m_obj.setProperty(m_key, value);
    }

private:
    QJSValue m_obj;
    QString m_key;
};

class JSValueCustomAccessorDescriptor : public QObject {
    Q_OBJECT
    using GetterFunc = std::function<QJSValue ()>;
    using SetterFunc = std::function<void (const QJSValue &)>;
public:
    explicit JSValueCustomAccessorDescriptor(const GetterFunc &getter, const SetterFunc &setter): QObject(nullptr), getter(getter), setter(setter) {
    }

public slots:
    QJSValue get() const {
        return getter();
    }
    void set(const QJSValue &value) {
        setter(value);
    }

private:
    GetterFunc getter;
    SetterFunc setter;
};

class ObjectWrapper {
public:
    static QStringList qWidgetGeneralKeys();

    static QJSValue wrap(QObject *obj, QJSEngine *engine, const QStringList &includedKeys = {}, const QStringList &excludedKeys = {});

    template<class Obj, class Ret, class FuncClass, class... Args>
    static void bindSignal(Obj *obj, Ret (FuncClass::*func) (Args...), QJSValue wrapped, const QString &name) {
        QString key = "on" + name;
        key[2] = key[2].toUpper();
        wrapped.setProperty(key, QJSValue::NullValue);
        QObject::connect(obj, func, [=](Args... args) {
            QJSValueList argList = {args...};
            auto cb = wrapped.property(key);
            if (cb.isCallable())
                cb.call(argList);
        });
    }

    template<typename GetterFunctor, typename SetterFunctor>
    static void addAccessorProperty(QJSValue wrapped, QJSEngine *engine, const QString &key, GetterFunctor getterFunctor, SetterFunctor setterFunctor) {
        addAccessorPropertyImpl(wrapped, engine, key, new JSValueCustomAccessorDescriptor(getterFunctor, setterFunctor));
    }

private:
    static void addAccessorPropertyImpl(QJSValue wrapped, QJSEngine *engine, const QString &key, JSValueCustomAccessorDescriptor *descriptorObject);
};

#define OBJECT_WRAPPER_BIND_SIGNAL(obj, wrapped, signal) ObjectWrapper::bindSignal(obj, &std::remove_pointer<decltype(obj)>::type::signal, wrapped, #signal)


#endif // DIFFSCOPE_OBJECTWRAPPER_H
