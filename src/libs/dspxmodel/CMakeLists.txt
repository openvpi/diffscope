project(dspxmodel VERSION 0.0.1.0)

# Add target
ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

# Configure target
file(GLOB_RECURSE _src *.h *.cpp)
qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core
    LINKS substate::qsubstate svscraft::Core
    INCLUDE_PRIVATE
    SKIP_AUTOMOC "dspxmodelglobal.h"
)

# Add install command
ck_sync_include(${PROJECT_NAME})