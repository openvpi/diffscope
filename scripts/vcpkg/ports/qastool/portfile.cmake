vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/qt-json-autogen
    REF 0.0.2.4
    SHA512 297a1b0e99cd8e27b66557a92e5dc22370ab9783c7e63b03e54d0fa1942a4c3fe8e094aa2fda59e865b529ad3acc831da0adf90e11baaf9180603182eadc5264
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DQAS_BUILD_EXAMPLES=OFF
        -DQAS_BUILD_MOC_EXE=OFF
        -DQAS_VCPKG_TOOLS_HINT=ON
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME qastool
    CONFIG_PATH lib/cmake/${PORT}
)

if(APPLE)
    get_filename_component(qt_lib_dir "$ENV{QT_DIR}/../.." REALPATH)
    execute_process(COMMAND install_name_tool -add_rpath "${qt_lib_dir}" "${CURRENT_PACKAGES_DIR}/tools/qastool/qasc")
endif()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/lib")
file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/debug")
file(COPY "${CURRENT_PACKAGES_DIR}/tools" DESTINATION "${CURRENT_PACKAGES_DIR}/debug")

vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)