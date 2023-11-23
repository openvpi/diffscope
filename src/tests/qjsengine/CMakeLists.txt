project(tst_QJSEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE _src *.h *.cpp)

ck_add_executable(${PROJECT_NAME} ${_src} Res/res.qrc)

ck_target_components(${PROJECT_NAME}
    LINKS QMCore
    QT_LINKS Core Gui Widgets Qml Xml Svg
)