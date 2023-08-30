project(dspxmodel VERSION 0.0.0.1 LANGUAGES CXX)

file(GLOB _src *.h *.cpp)

find_package(acetree REQUIRED)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN
    DESCRIPTION "DiffScope Project Model"
)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    LINKS acetree ChorusKit::SVSBasic
    AUTO_INCLUDE_CURRENT
)