#ifndef INITROUTINE_H
#define INITROUTINE_H

#include <functional>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSplashScreen>

#include <coreplugin/coreglobal.h>

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class CORE_EXPORT InitRoutine {
    public:
        enum StartMode {
            Application,
            VST,
        };
        using StartEntry = std::function<QWidget *()>;

    public:
        static QSplashScreen *splash();

        static int startMode();
        static void setStartMode(StartMode startMode);

        static StartEntry startEntry();
        static void setStartEntry(const StartEntry &startEntry);
    };

}

#endif // INITROUTINE_H
