#include "TempoList.h"

#include <QQmlEngine>

#include <dspxmodel/timelineentity.h>

#include <scriptmanager/internal/Tempo.h>

namespace ScriptManager::Internal {

    SHEET_ENTITY_WRAPPER_IMPLEMENTATION_WRAPPED(TempoList, QDspx::TempoListEntity, Tempo)

}