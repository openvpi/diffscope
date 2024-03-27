add_subdirectory(dspxmodel)

add_subdirectory(appshared)

if(WIN32 OR APPLE)
    add_subdirectory(widgetframe)
endif()