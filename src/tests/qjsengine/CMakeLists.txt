project(tst_QJSEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB _src *.h *.cpp)

ck_add_executable(${PROJECT_NAME} ${_src} res.qrc)

ck_target_components(${PROJECT_NAME}
    QT_LINKS Core Gui Widgets Qml
)