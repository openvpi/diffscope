project(tst_talcs)

file(GLOB _src *.h *.cpp)

ck_add_executable(${PROJECT_NAME} ${_src})

ck_target_components(${PROJECT_NAME}
    LINKS talcs
)