#ifndef APPEXTRA_H
#define APPEXTRA_H

#include <QtWidgets/QMenu>

#include <coreplugin/coreglobal.h>

namespace Core {

    class CORE_EXPORT AppExtra {
    public:
        static QMenu *createCoreMenu(QWidget *parent = nullptr);

        static void autoPolishPopupMenu(QWidget *w);
        static void autoPolishScrollArea(QWidget *w);
        static void autoPolishScrollBars(QWidget *w);

        static QString fileManagerName();
        static inline QString allFilesFilter();
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

}

#endif // APPEXTRA_H
