project (QHotkey)

add_library(${PROJECT_NAME}
    qhotkey.cpp
    qhotkey.h
    qhotkey_p.h
    qhotkey_mac.cpp qhotkey_win.cpp qhotkey_x11.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

if (CMAKE_HOST_UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE X11)
endif()
