qm_import(Translate)
qm_find_qt(LinguistTools)

# Add translation
file(GLOB _qt_ts_files qt/translations/*.ts)
qm_add_translation(Qt_translations
    TS_FILES ${_qt_ts_files}
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/qt/translations
    QM_DEPENDS ChorusKit_ReleaseTranslations
)

# Deploy
ck_add_shared_files(
    SRC ${CMAKE_CURRENT_BINARY_DIR}/qt/translations DEST ${CK_BUILD_MAIN_DIR}/share/qt
)
