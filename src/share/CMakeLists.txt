qm_import(Translate)
qm_find_qt(LinguistTools)

# Add translation
file(GLOB _qt_ts_files qt/translations/*.ts)
qm_add_translation(Qt_translations
    TS_FILES ${_qt_ts_files}
    QM_DIR ${CK_BUILD_MAIN_DIR}/share/qt/translations
    QM_DEPENDS ChorusKit_ReleaseTranslations
)

# Deploy
ck_add_shared_files(
    SKIP_BUILD
    SRC ${CK_BUILD_MAIN_DIR}/share/qt/translations DEST ${CK_BUILD_MAIN_DIR}/share/qt
)
