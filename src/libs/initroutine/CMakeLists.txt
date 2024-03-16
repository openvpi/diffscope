project(initroutine VERSION 0.0.1.0)

# Add target
ck_add_library(${PROJECT_NAME} INTERFACE)

# Configure target
qm_link_qt(${PROJECT_NAME} INTERFACE Core Gui Widgets)

target_link_libraries(${PROJECT_NAME} INTERFACE ChorusKit::AppCore)

# Add install command
ck_sync_include(${PROJECT_NAME})