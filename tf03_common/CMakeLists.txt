configure_file(
  config.h.in
    "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY
)

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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Charts REQUIRED)

add_library(
  tf03_common
    SHARED
      driver.h
      driver.cpp
      driver_commands.cpp
      driver_parsers.cpp
      devel_mode_task.cpp
      devel_mode_task.h
      parsed.cpp
      parsed.h
      config.h.in
      export.h
      lingual.h
      lingual.cpp
      plot_base.h
      plot_base.cpp
      distance_over_time_chart.h
      distance_over_time_chart.cpp
      apd_page.h
      apd_page.cpp
      command_echo_handler.cpp
      command_echo_handler.h
      utils.h
      utils.cpp
      measure_manifest.h
      measure_manifest.cpp
      firmware_module.h
      firmware_module.cpp
      command_echo_widgets.cpp
      command_echo_widgets.h
      command_echo_widgets_manager.cpp
      command_echo_widgets_manager.h
      apd_exp_task.cpp
      apd_exp_task.h
      range_detect_task.cpp
      range_detect_task.h
      page_base.cpp
      page_base.h
      apd_page_wrapper.cpp
      apd_page_wrapper.h
      connection_page.cpp
      connection_page.h
      firmware_update_page.cpp
      firmware_update_page.h
)

target_link_libraries(
  tf03_common
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Charts
)

set(
  PUBLIC_HEADERS
    export.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set_target_properties(tf03_common PROPERTIES
  PUBLIC_HEADER
    "${PUBLIC_HEADERS}"
)

install(TARGETS tf03_common
  # IMPORTANT: Add the tf03_common library to the "export-set"
  EXPORT tf03_sdkTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/tf03_common"
    COMPONENT dev
)