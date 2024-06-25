#ifndef SCRIPT_MANAGER_TEMPOLIST_H
#define SCRIPT_MANAGER_TEMPOLIST_H

#include <scriptmanager/internal/SheetEntityWrapper.h>

namespace QDspx {
    class TempoListEntity;
}

namespace ScriptManager::Internal {

    class TempoList : public SheetEntityWrapper {
        Q_OBJECT
        SHEET_ENTITY_WRAPPER_DECLARATION(TempoList, QDspx::TempoListEntity)
    };

}

#endif // SCRIPT_MANAGER_TEMPOLIST_H
