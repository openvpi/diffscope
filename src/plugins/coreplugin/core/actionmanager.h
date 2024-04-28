#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <optional>

#include <QObject>
#include <QSettings>

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

        enum Scope {
            System,
            User,
        };

    public:
        ActionDomain *domain() const;

    public:
        bool loadLayouts();
        bool saveLayouts() const;

        bool loadIcons();
        bool saveIcons() const;

        bool loadShortcuts();
        bool saveShortcuts() const;

        QPair<QString, Scope> currentShortcutsFamily() const;
        void setCurrentShortcutsFamily(const QString &id, Scope scope);

        ActionDomain::ShortcutsFamily shortcutsFamily(const QString &id, Scope scope = System) const;
        QStringList shortcutFamilies(Scope scope = System) const;
        void addShortcutsFamily(const QString &id, const ActionDomain::ShortcutsFamily &family,
                               Scope scope = System);
        void removeShortcutsFamily(const QString &id, Scope scope = System);
        void clearShortcutsFamilies(Scope scope = System);

    Q_SIGNALS:
        void layoutsReloaded();
        void shortcutsReloaded();

    protected:
        QScopedPointer<ActionManagerPrivate> d_ptr;
    };

}

#endif // ACTIONMANAGER_H
