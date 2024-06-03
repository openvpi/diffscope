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

    public:
        ActionDomain *domain() const;

    public:
        bool loadLayouts();
        bool saveLayouts() const;

        bool loadIcons();
        bool saveIcons() const;

        bool loadShortcuts();
        bool saveShortcuts() const;

        QPair<QString, bool> currentShortcutsFamily() const;
        bool setCurrentShortcutsFamily(const QString &id, bool system);

        QStringList systemShortcutsFamilies() const;
        ActionDomain::ShortcutsFamily systemShortcutsFamily(const QString &id) const;
        bool addSystemShortcutsFamily(const QString &id,
                                      const ActionDomain::ShortcutsFamily &family);
        bool removeSystemShortcutsFamily(const QString &id);
        void clearSystemShortcutsFamily();

        QStringList userShortcutFamilies() const;
        ActionDomain::ShortcutsFamily userShortcutsFamily(const QString &id) const;
        bool addUserShortcutsFamily(const QString &id, const ActionDomain::ShortcutsFamily &family);
        bool renameUserShortcutsFamily(const QString &id, const QString &newId);
        bool removeUserShortcutsFamily(const QString &id);
        void clearUserShortcutsFamilies();

    Q_SIGNALS:
        void layoutsReloaded();
        void shortcutsReloaded();

    protected:
        QScopedPointer<ActionManagerPrivate> d_ptr;
    };

}

#endif // ACTIONMANAGER_H
