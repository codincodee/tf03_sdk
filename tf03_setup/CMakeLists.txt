configure_file(
  config.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Charts REQUIRED)
# find_package(Qt5Charts)

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

add_executable(
  tf03_setup
  WIN32
    main.cpp
    driver_parsers.cpp
    driver_commands.cpp
    driver.cpp
    driver.h
    parsed.cpp
    parsed.h
    main_window.cpp
    main_window.h
    main_window.ui
    static_unique_ptr_cast.h
    lingual.cpp
    lingual.h
    command_echo_widgets.cpp
    command_echo_widgets.h
    command_echo_widgets_manager.cpp
    command_echo_widgets_manager.h
    command_echo_handler.cpp
    command_echo_handler.h
    firmware_module.cpp
    firmware_module.h
    config.h.in
    distance_over_time_chart.cpp
    distance_over_time_chart.h
    apd_page.cpp
    apd_page.h
    utils.cpp
    utils.h
    devel_mode_task.cpp
    devel_mode_task.h
    apd_exp_task.cpp
    apd_exp_task.h
    measure_manifest.cpp
    measure_manifest.h
    plot_base.cpp
    plot_base.h
    resource.qrc
)

target_link_libraries(
  tf03_setup
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::WinMain
    Qt5::Charts
)

install(TARGETS tf03_setup
  # IMPORTANT: Add the tf03_setup executable to the "export-set"
  EXPORT tf03_sdkTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
