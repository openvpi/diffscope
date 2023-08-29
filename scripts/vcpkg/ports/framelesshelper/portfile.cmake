vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO wangwenx190/framelesshelper
    REF 2.4.2
    SHA512 f7ee92a790c0a8d2e9791745682a874a90d5ed6dffb54d2e71775ad9a84710352c5e30caee10964fdd8262706ff19056ea1292337e8a9bf3dc7260a50378a702
)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/cmake "${SOURCE_PATH}/cmake")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DFRAMELESSHELPER_BUILD_STATIC=ON
        -DFRAMELESSHELPER_BUILD_WIDGETS=ON
        -DFRAMELESSHELPER_BUILD_QUICK=OFF
        -DFRAMELESSHELPER_BUILD_EXAMPLES=OFF
        -DFRAMELESSHELPER_NO_DEBUG_OUTPUT=ON
        -DFRAMELESSHELPER_NO_BUNDLE_RESOURCE=ON
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/FramelessHelper)
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)

file(REMOVE "${CURRENT_PACKAGES_DIR}/FramelessHelper.props")
file(REMOVE "${CURRENT_PACKAGES_DIR}/debug/FramelessHelper.props")