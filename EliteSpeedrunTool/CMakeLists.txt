cmake_minimum_required(VERSION 3.5)

project(EliteSpeedrunTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS HttpServer)
find_package(Qt6 REQUIRED COMPONENTS StateMachine)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp MainWindow.h MainWindow.ui
        FirewallUtil.h FirewallUtil.cpp
        GlobalData.h GlobalData.cpp
        SettingDialog.h SettingDialog.cpp SettingDialog.ui
        DisplayInfoDialog.h DisplayInfoDialog.cpp DisplayInfoDialog.ui
        UpdateDialog.h UpdateDialog.cpp UpdateDialog.ui
        DescriptionDialog.h DescriptionDialog.cpp DescriptionDialog.ui
        AcknowledgementDialog.h AcknowledgementDialog.cpp AcknowledgementDialog.ui
        LogUtil.h LogUtil.cpp
        LanguageUtil.h LanguageUtil.cpp
        HttpServerUtil.h HttpServerUtil.cpp
        DisplayInfoSubFunction.h DisplayInfoSubFunction.cpp
        DiscordUtil.h DiscordUtil.cpp
        TimeUtil.h TimeUtil.cpp
        SuspendUtil.h SuspendUtil.cpp
        NumberUtil.h NumberUtil.cpp
        resource.qrc
)

if (WIN32)
    list (APPEND PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/App.rc")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EliteSpeedrunTool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EliteSpeedrunTool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EliteSpeedrunTool SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EliteSpeedrunTool
            ${PROJECT_SOURCES}
        )
    endif()
endif()

file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/*.ts")

qt_add_lupdate(EliteSpeedrunTool TS_FILES en.ts OPTIONS "-no-obsolete")
install(FILES ${qm_files} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})


target_link_libraries(EliteSpeedrunTool PRIVATE Qt6::Widgets)
target_link_libraries(EliteSpeedrunTool PRIVATE Qt6::HttpServer)
target_link_libraries(EliteSpeedrunTool PRIVATE Qt6::StateMachine)


set_target_properties(EliteSpeedrunTool PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER https://github.com/SkyD666
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS EliteSpeedrunTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EliteSpeedrunTool)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../QHotkey ${CMAKE_CURRENT_BINARY_DIR}/QHotkey)
include_directories(../QHotkey)
target_link_libraries(EliteSpeedrunTool PRIVATE QHotkey)

if (WIN32)
    target_link_libraries(EliteSpeedrunTool PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/WinMM.Lib
    )
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../DiscordGameSDK ${CMAKE_CURRENT_BINARY_DIR}/DiscordGameSDK)
include_directories(../DiscordGameSDK)
target_link_libraries(EliteSpeedrunTool PRIVATE DiscordGameSDK)
