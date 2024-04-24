#ifndef APPEXTRA_H
#define APPEXTRA_H

#include <QtWidgets/QMenu>

#include <CoreApi/actionitem.h>

#include <coreplugin/coreglobal.h>

namespace Core {

    class CORE_EXPORT AppExtra {
    public:
        static QString mainTitle();
        static QString displayTitle(const QString &text);
        static void aboutApp(QWidget *parent = nullptr);
        static void fatalError(QWidget *parent, const QString &text, int exitCode = -1);

        static QMenu *createCoreMenu(QWidget *parent = nullptr);

        static void autoPolishPopupMenu(QWidget *w);
        static void autoPolishScrollArea(QWidget *w);
        static void autoPolishScrollBars(QWidget *w);

        static QString fileManagerName();
        static inline QString allFilesFilter();

        static inline ActionItem *actionItem(const QString &id, QObject *parent = nullptr);
        static inline ActionItem *actionItem(const QString &id, QAction::MenuRole role,
                                             QObject *parent = nullptr);
        static inline ActionItem *menuItem(const QString &id, QObject *parent = nullptr);
    };

    inline QString AppExtra::allFilesFilter() {
        return
#ifdef Q_OS_WINDOWS
            QStringLiteral("*.*")
#else
            QStringLiteral("*")
#endif
                ;
    }

    inline ActionItem *AppExtra::actionItem(const QString &id, QObject *parent) {
        return actionItem(id, QAction::NoRole, parent);
    }

    inline ActionItem *AppExtra::actionItem(const QString &id, QAction::MenuRole role,
                                            QObject *parent) {
        auto action = new QAction();
        action->setMenuRole(role);
        return new ActionItem(id, action, parent);
    }

    inline ActionItem *AppExtra::menuItem(const QString &id, QObject *parent) {
        return new ActionItem(id, ActionItem::MenuFactory(createCoreMenu), parent);
    }

}

#endif // APPEXTRA_H
