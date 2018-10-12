set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if (SUPPORT_DEVEL_MODE_PROTOCOL)
  add_definitions(-DSUPPORT_DEVEL_MODE_PROTOCOL_)
#  add_definitions(-DDISPLAY_PLOT_ON_SETUP_PAGE)
endif()

if (USE_APD_EXPERIMENT_PAGE)
  add_definitions(-DSUPPORT_DEVEL_MODE_PROTOCOL_)
  add_definitions(-DUSE_APD_EXPERIMENT_PAGE)
endif()

if (AUTOMATIC_SERIAL_CONNECTION)
  add_definitions(-DAUTOMATIC_SERIAL_CONNECTION)
endif()

if (CLIENT_BL_CUSTOMIZATION)
  add_definitions(-DCLIENT_BL_CUSTOMIZATION)
endif()

if (DISPLAY_PLOT_ON_SETUP_PAGE)
  add_definitions(-DDISPLAY_PLOT_ON_SETUP_PAGE)
endif()

if (SWITCH_ON_ALL_FEATURES)
  add_definitions(-DSWITCH_ON_ALL_FEATURES)
  add_definitions(-DSUPPORT_DEVEL_MODE_PROTOCOL_)
  add_definitions(-DUSE_APD_EXPERIMENT_PAGE)
  add_definitions(-DAUTOMATIC_SERIAL_CONNECTION)
  add_definitions(-DCLIENT_BL_CUSTOMIZATION)
  add_definitions(-DDISPLAY_PLOT_ON_SETUP_PAGE)
endif()

find_package(Qt5Widgets)

add_executable(
  tf03_production
  WIN32
    main.cpp
    main_window.cpp
    main_window.h
    main_window.ui
    resource.qrc
)

target_link_libraries(
  tf03_production
    Qt5::WinMain
    tf03_common
)

install(TARGETS tf03_production
  # IMPORTANT: Add the tf03_production executable to the "export-set"
  EXPORT tf03_sdkTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
