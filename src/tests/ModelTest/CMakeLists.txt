project(tst_ModelTest)

file(GLOB_RECURSE _src *.h *.cpp)

ck_add_executable(${PROJECT_NAME} AUTOGEN CONSOLE ${_src})

qm_configure_target(${PROJECT_NAME}
    LINKS dspxmodel
    QT_LINKS Core Gui Widgets
)