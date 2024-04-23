#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>

#include <CoreApi/actiondomain.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class ActionManagerPrivate;

    class CORE_EXPORT ActionManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ActionManager)
    public:
        explicit ActionManager(QObject *parent = nullptr);
        ~ActionManager();

        static ActionManager *instance();

    public:
        ActionDomain *domain() const;

    public:
        bool loadLayouts();
        bool saveLayouts() const;

        bool loadIcons();
        bool saveIcons() const;

        bool loadShortcuts();
        bool saveShortcuts() const;

    Q_SIGNALS:
        void layoutsReloaded();
        void shortcutsReloaded();

    protected:
        QScopedPointer<ActionManagerPrivate> d_ptr;
        ActionManager(ActionManagerPrivate &d, QObject *parent = nullptr);
    };

}

#endif // ACTIONMANAGER_H
