#ifndef INITROUTINE_H
#define INITROUTINE_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>

#include <CoreApi/iloader.h>

namespace Core {

    struct InitRoutine {
        enum StartMode {
            Application,
            VST,
        };

        using StartEntry = void (*)();

        inline InitRoutine() {
            ILoader::quickData(0) = this;
        }

        inline ~InitRoutine() {
            ILoader::quickData(0) = nullptr;
        }

        static InitRoutine *instance() {
            return static_cast<InitRoutine *>(ILoader::quickData(0));
        }

        QSplashScreen *splash = nullptr;
        int startMode = Application;
        StartEntry entry = nullptr;
    };

}

#endif // INITROUTINE_H
