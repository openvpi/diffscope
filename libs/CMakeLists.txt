set(QTMEDIATE_INSTALL off)
set(CHORUSKIT_INSTALL off)
set(SVSCRAFT_INSTALL off)
set(OPENDSPX_INSTALL off)
set(JBDS_INSTALL off)
set(SUBSTATE_INSTALL off)
set(TALCS_INSTALL off)
set(IDECORE_INSTALL off)

add_subdirectory(qtmediate)

add_subdirectory(choruskit)

add_subdirectory(svscraft)

add_subdirectory(opendspx)

add_subdirectory(jetbrains-docking-system)

add_subdirectory(substate)

set(TALCS_DSPX ON)
set(TALCS_REMOTE ON)
add_subdirectory(talcs)

add_subdirectory(idecore)

add_subdirectory(stimmt)

set(CK_CMAKE_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/choruskit/cmake PARENT_SCOPE)

set(CK_CKAEC_EXECUTABLE $<TARGET_FILE:ckaec> PARENT_SCOPE)