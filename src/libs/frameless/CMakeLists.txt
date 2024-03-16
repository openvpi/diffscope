project(frameless VERSION 0.0.1.0)

# Add target
ck_add_library(${PROJECT_NAME} STATIC AUTOGEN
    SKIP_EXPORT
    SKIP_INSTALL
)

# Configure target
file(GLOB_RECURSE _src *.h *.cpp)
qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Gui Widgets
    LINKS QWindowKit::Widgets qtmediate::Widgets
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR})