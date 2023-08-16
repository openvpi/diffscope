set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries/lib)

# set(QTMEDIATE_INSTALL off)
add_subdirectory(qtmediate)

# set(CHORUSKIT_INSTALL off)
add_subdirectory(choruskit)