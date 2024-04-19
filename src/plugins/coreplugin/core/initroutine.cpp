#include "initroutine.h"
#include "initroutine_p.h"

namespace Core {

    namespace Internal {
        extern InitRoutinePrivate *ir;
    }

    QSplashScreen *InitRoutine::splash() {
        return Internal::ir->splash;
    }

    int InitRoutine::startMode() {
        return Internal::ir->startMode;
    }

    void InitRoutine::setStartMode(InitRoutine::StartMode startMode) {
        Internal::ir->startMode = startMode;
    }

    InitRoutine::StartEntry InitRoutine::startEntry() {
        return Internal::ir->entry;
    }

    void InitRoutine::setStartEntry(const InitRoutine::StartEntry &startEntry) {
        Internal::ir->entry = startEntry;
    }

}