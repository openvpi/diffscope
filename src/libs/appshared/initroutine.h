#ifndef INITROUTINE_H
#define INITROUTINE_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>

#include <CoreApi/iloader.h>

namespace AppShared {

    struct InitRoutine {
        enum StartMode {
            Application,
            VST,
        };

        using StartEntry = QWidget *(*) ();

        static constexpr const int InitRoutineDataIndex = 0;

        inline InitRoutine() {
            Core::ILoader::quickData(InitRoutineDataIndex) = this;
        }

        inline ~InitRoutine() {
            Core::ILoader::quickData(InitRoutineDataIndex) = nullptr;
        }

        static InitRoutine *instance() {
            return static_cast<InitRoutine *>(Core::ILoader::quickData(InitRoutineDataIndex));
        }

        QSplashScreen *splash = nullptr;
        int startMode = Application;
        StartEntry entry = nullptr;
    };

}

#endif // INITROUTINE_H
