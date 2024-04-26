#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <optional>

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

    public:
        ActionDomain *domain() const;

    public:
        bool loadLayouts();
        bool saveLayouts() const;

        bool loadIcons();
        bool saveIcons() const;

        bool loadShortcuts();
        bool saveShortcuts() const;

        QString currentShortcutFamily() const;
        void setCurrentShortcutFamily(const QString &id);

        QJsonObject shortcutFamily(const QString &id) const;
        QStringList shortcutFamilies() const;
        void addShortcutFamily(const QString &id, const QJsonObject &family);
        void removeShortcutFamily(const QString &id);
        void clearShortcutFamilies();

    Q_SIGNALS:
        void layoutsReloaded();
        void shortcutsReloaded();

    protected:
        QScopedPointer<ActionManagerPrivate> d_ptr;
        ActionManager(ActionManagerPrivate &d, QObject *parent = nullptr);
    };

}

#endif // ACTIONMANAGER_H
