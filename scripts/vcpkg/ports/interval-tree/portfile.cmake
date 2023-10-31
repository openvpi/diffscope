vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO CrSjimo/interval-tree
    REF ee6db529cc58d8d0e19ddaf6c3a9d3c652d5fce5
    SHA512 385bd4ab1ef2ec3ac85b55eb0e4bdabd246eecfc58e0129685bf38ceb5eb6680f332bdfb59333c010cbbaa2de3541bf795b7c0877f8e634eedd5a7a4f798d521
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)