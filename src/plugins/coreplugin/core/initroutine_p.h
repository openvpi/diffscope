#ifndef INITROUTINE_P_H
#define INITROUTINE_P_H

#include <coreplugin/initroutine.h>

namespace Core {

    struct InitRoutinePrivate {
    public:
        QSplashScreen *splash = nullptr;
        int startMode = InitRoutine::Application;
        InitRoutine::StartEntry entry;
    };

}

#endif // INITROUTINE_P_H
