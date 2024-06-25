#include "TimelineLabelList.h"

#include <QQmlEngine>

#include <dspxmodel/timelineentity.h>

#include <scriptmanager/internal/TimelineLabel.h>

namespace ScriptManager::Internal {

    SHEET_ENTITY_WRAPPER_IMPLEMENTATION_WRAPPED(TimelineLabelList, QDspx::TimelineLabelListEntity, TimelineLabel)

}