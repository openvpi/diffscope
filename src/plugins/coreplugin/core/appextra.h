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
    };

}

#endif // APPEXTRA_H
