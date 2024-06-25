#ifndef SCRIPT_MANAGER_TIMELINELABELLIST_H
#define SCRIPT_MANAGER_TIMELINELABELLIST_H

#include <scriptmanager/internal/SheetEntityWrapper.h>

namespace QDspx {
    class TimelineLabelListEntity;
}

namespace ScriptManager::Internal {

    class TimelineLabelList : public SheetEntityWrapper {
        Q_OBJECT
        SHEET_ENTITY_WRAPPER_DECLARATION(TimelineLabelList, QDspx::TimelineLabelListEntity)
    };

}

#endif // SCRIPT_MANAGER_TIMELINELABELLIST_H
