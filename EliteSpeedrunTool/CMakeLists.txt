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
        MemoryUtil.h MemoryUtil.cpp
        HttpServerUtil.h HttpServerUtil.cpp
        DisplayInfoSubFunction.h DisplayInfoSubFunction.cpp
        DiscordUtil.h DiscordUtil.cpp
        TimeUtil.h TimeUtil.cpp
        LottieUtil.h LottieUtil.cpp
        dataobserver/AutoTimerUtil.h dataobserver/AutoTimerUtil.cpp
        dataobserver/DataObserver.h dataobserver/DataObserver.cpp
        #=================
        dataobserver/missionstrategy/MissionStrategyUtil.h dataobserver/missionstrategy/MissionStrategyUtil.cpp
        dataobserver/missionstrategy/BaseMissionStrategy.h dataobserver/missionstrategy/BaseMissionStrategy.cpp

        dataobserver/missionstrategy/act1/TheDataBreachesStrategy.h dataobserver/missionstrategy/act1/TheDataBreachesStrategy.cpp
        dataobserver/missionstrategy/act1/Act1DeadCourierStrategy.h dataobserver/missionstrategy/act1/Act1DeadCourierStrategy.cpp
        dataobserver/missionstrategy/act1/Act1SignalInterceptsStrategy.h dataobserver/missionstrategy/act1/Act1SignalInterceptsStrategy.cpp
        dataobserver/missionstrategy/act1/Act1ServerFarmStrategy.h dataobserver/missionstrategy/act1/Act1ServerFarmStrategy.cpp

        dataobserver/missionstrategy/act2/TheBogdanProblemStrategy.h dataobserver/missionstrategy/act2/TheBogdanProblemStrategy.cpp
        dataobserver/missionstrategy/act2/Act2AvengerStrategy.h dataobserver/missionstrategy/act2/Act2AvengerStrategy.cpp
        dataobserver/missionstrategy/act2/Act2RescueUlpStrategy.h dataobserver/missionstrategy/act2/Act2RescueUlpStrategy.cpp
        dataobserver/missionstrategy/act2/Act2SalvageHardDrivesStrategy.h dataobserver/missionstrategy/act2/Act2SalvageHardDrivesStrategy.cpp
        dataobserver/missionstrategy/act2/Act2SubmarineReconStrategy.h dataobserver/missionstrategy/act2/Act2SubmarineReconStrategy.cpp

        dataobserver/missionstrategy/act3/TheDoomsdayScenarioStrategy.h dataobserver/missionstrategy/act3/TheDoomsdayScenarioStrategy.cpp
        dataobserver/missionstrategy/act3/Act3RescueAgent14Strategy.h dataobserver/missionstrategy/act3/Act3RescueAgent14Strategy.cpp
        dataobserver/missionstrategy/act3/Act3EscortUlpStrategy.h dataobserver/missionstrategy/act3/Act3EscortUlpStrategy.cpp
        dataobserver/missionstrategy/act3/Act3BarrageStrategy.h dataobserver/missionstrategy/act3/Act3BarrageStrategy.cpp
        dataobserver/missionstrategy/act3/Act3KhanjaliStrategy.h dataobserver/missionstrategy/act3/Act3KhanjaliStrategy.cpp
        dataobserver/missionstrategy/act3/Act3AirDefensesStrategy.h dataobserver/missionstrategy/act3/Act3AirDefensesStrategy.cpp

        dataobserver/missionstrategy/LostMcRipStrategy.h dataobserver/missionstrategy/LostMcRipStrategy.cpp

        dataobserver/missionstrategy/EmptyStrategy.h dataobserver/missionstrategy/EmptyStrategy.cpp

        dataobserver/missionstrategy/fleecajob/TheFleecaJobStrategy.h dataobserver/missionstrategy/fleecajob/TheFleecaJobStrategy.cpp
        dataobserver/missionstrategy/fleecajob/FleecaJobScopeOutStrategy.h dataobserver/missionstrategy/fleecajob/FleecaJobScopeOutStrategy.cpp
        dataobserver/missionstrategy/fleecajob/FleecaJobKurumaStrategy.h dataobserver/missionstrategy/fleecajob/FleecaJobKurumaStrategy.cpp

        dataobserver/missionstrategy/prisonbreak/PrisonBreakPlaneStrategy.h dataobserver/missionstrategy/prisonbreak/PrisonBreakPlaneStrategy.cpp
        dataobserver/missionstrategy/prisonbreak/PrisonBreakBusStrategy.h dataobserver/missionstrategy/prisonbreak/PrisonBreakBusStrategy.cpp
        dataobserver/missionstrategy/prisonbreak/PrisonBreakStationStrategy.h dataobserver/missionstrategy/prisonbreak/PrisonBreakStationStrategy.cpp
        dataobserver/missionstrategy/prisonbreak/PrisonBreakWetWorkStrategy.h dataobserver/missionstrategy/prisonbreak/PrisonBreakWetWorkStrategy.cpp
        dataobserver/missionstrategy/prisonbreak/ThePrisonBreakStrategy.h dataobserver/missionstrategy/prisonbreak/ThePrisonBreakStrategy.cpp

        dataobserver/missionstrategy/seriesa/SeriesACokeStrategy.h dataobserver/missionstrategy/seriesa/SeriesACokeStrategy.cpp
        dataobserver/missionstrategy/seriesa/SeriesATrashTruckStrategy.h dataobserver/missionstrategy/seriesa/SeriesATrashTruckStrategy.cpp
        dataobserver/missionstrategy/seriesa/SeriesABikersStrategy.h dataobserver/missionstrategy/seriesa/SeriesABikersStrategy.cpp
        dataobserver/missionstrategy/seriesa/SeriesAWeedStrategy.h dataobserver/missionstrategy/seriesa/SeriesAWeedStrategy.cpp
        dataobserver/missionstrategy/seriesa/SeriesAStealMethStrategy.h dataobserver/missionstrategy/seriesa/SeriesAStealMethStrategy.cpp
        dataobserver/missionstrategy/seriesa/SeriesAFundingStrategy.h dataobserver/missionstrategy/seriesa/SeriesAFundingStrategy.cpp

        dataobserver/missionstrategy/humanelabsraid/HumaneRaidKeyCodesStrategy.h dataobserver/missionstrategy/humanelabsraid/HumaneRaidKeyCodesStrategy.cpp
        dataobserver/missionstrategy/humanelabsraid/HumaneRaidInsurgentsStrategy.h dataobserver/missionstrategy/humanelabsraid/HumaneRaidInsurgentsStrategy.cpp
        dataobserver/missionstrategy/humanelabsraid/HumaneRaidEmpStrategy.h dataobserver/missionstrategy/humanelabsraid/HumaneRaidEmpStrategy.cpp
        dataobserver/missionstrategy/humanelabsraid/HumaneRaidValkyrieStrategy.h dataobserver/missionstrategy/humanelabsraid/HumaneRaidValkyrieStrategy.cpp
        dataobserver/missionstrategy/humanelabsraid/HumaneRaidDeliverEmpStrategy.h dataobserver/missionstrategy/humanelabsraid/HumaneRaidDeliverEmpStrategy.cpp
        dataobserver/missionstrategy/humanelabsraid/TheHumaneLabsRaidStrategy.h dataobserver/missionstrategy/humanelabsraid/TheHumaneLabsRaidStrategy.cpp

        dataobserver/missionstrategy/pacificstandard/PacificStandardVansStrategy.h dataobserver/missionstrategy/pacificstandard/PacificStandardVansStrategy.cpp
        dataobserver/missionstrategy/pacificstandard/PacificStandardSignalStrategy.h dataobserver/missionstrategy/pacificstandard/PacificStandardSignalStrategy.cpp
        dataobserver/missionstrategy/pacificstandard/PacificStandardHackStrategy.h dataobserver/missionstrategy/pacificstandard/PacificStandardHackStrategy.cpp
        dataobserver/missionstrategy/pacificstandard/PacificStandardConvoyStrategy.h dataobserver/missionstrategy/pacificstandard/PacificStandardConvoyStrategy.cpp
        dataobserver/missionstrategy/pacificstandard/PacificStandardBikesStrategy.h dataobserver/missionstrategy/pacificstandard/PacificStandardBikesStrategy.cpp
        dataobserver/missionstrategy/pacificstandard/ThePacificStandardStrategy.h dataobserver/missionstrategy/pacificstandard/ThePacificStandardStrategy.cpp

        dataobserver/missionstrategy/casinoheist/CasinoHeistTheBigConStrategy.h dataobserver/missionstrategy/casinoheist/CasinoHeistTheBigConStrategy.cpp
        dataobserver/missionstrategy/casinoheist/CasinoHeistSilentSneakyStrategy.h dataobserver/missionstrategy/casinoheist/CasinoHeistSilentSneakyStrategy.cpp
        dataobserver/missionstrategy/casinoheist/CasinoHeistAggressiveStrategy.h dataobserver/missionstrategy/casinoheist/CasinoHeistAggressiveStrategy.cpp
        dataobserver/missionstrategy/casinoheist/CasinoHeistEndStageStrategy.h dataobserver/missionstrategy/casinoheist/CasinoHeistEndStageStrategy.cpp

        dataobserver/missionstrategy/casino/CasinoBadBeatStrategy.h dataobserver/missionstrategy/casino/CasinoBadBeatStrategy.cpp
        #=================
        dataobserver/datafetcher/BaseDataFetcher.h
        dataobserver/datafetcher/DataFetcherUtil.h dataobserver/datafetcher/DataFetcherUtil.cpp
        dataobserver/datafetcher/MissionHashFetcher.h dataobserver/datafetcher/MissionHashFetcher.cpp
        dataobserver/datafetcher/HeadshotFetcher.h dataobserver/datafetcher/HeadshotFetcher.cpp
        dataobserver/datafetcher/KillFetcher.h dataobserver/datafetcher/KillFetcher.cpp
        dataobserver/datafetcher/Kill1Fetcher.h dataobserver/datafetcher/Kill1Fetcher.cpp
        dataobserver/datafetcher/Kill2Fetcher.h dataobserver/datafetcher/Kill2Fetcher.cpp
        dataobserver/datafetcher/Kill3Fetcher.h dataobserver/datafetcher/Kill3Fetcher.cpp
        dataobserver/datafetcher/Kill4Fetcher.h dataobserver/datafetcher/Kill4Fetcher.cpp
        dataobserver/datafetcher/VehicleDamageFetcher.h dataobserver/datafetcher/VehicleDamageFetcher.cpp
        dataobserver/datafetcher/RashkovskyHurtFetcher.h dataobserver/datafetcher/RashkovskyHurtFetcher.cpp
        AutoCapture.h AutoCapture.cpp
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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../QtLottie ${CMAKE_CURRENT_BINARY_DIR}/QtLottie)
include_directories(../QtLottie)
target_link_libraries(EliteSpeedrunTool PRIVATE QtLottie)
