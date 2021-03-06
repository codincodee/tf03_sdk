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

if (ATTACH_RAW_MSG_TO_MEASURE)
  add_definitions(-DATTACH_RAW_MSG_TO_MEASURE)
endif()

if (SWITCH_ON_ALL_FEATURES)
  add_definitions(-DSWITCH_ON_ALL_FEATURES)
  add_definitions(-DSUPPORT_DEVEL_MODE_PROTOCOL_)
  add_definitions(-DUSE_APD_EXPERIMENT_PAGE)
  add_definitions(-DAUTOMATIC_SERIAL_CONNECTION)
  add_definitions(-DCLIENT_BL_CUSTOMIZATION)
  add_definitions(-DDISPLAY_PLOT_ON_SETUP_PAGE)
endif()

add_definitions(-DATTACH_RAW_MSG_TO_MEASURE)
add_definitions(-DAPD_EXPERIMENT_USE_RAWDIST2)

if (DISPLAY_SERIAL_IO_MESSAGE)
  add_definitions(-DDISPLAY_SERIAL_IO_MESSAGE_)
endif()

if (SUPPORT_10KHZ_OUTPUT)
  add_definitions(-DSUPPORT_10KHZ_OUTPUT)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Charts REQUIRED)

if("${MATLAB_PATH}" STREQUAL "")
else()
  message("Found MATLAB")
  include_directories("${MATLAB_PATH}/extern/include")
  if("${MATLAB_LIB_DIR}" STREQUAL "")
    set(MATLAB_LIB_DIR "${MATLAB_PATH}/extern/lib/win64/microsoft")
  endif()
  set(
    MATLAB_DLL_LIBS
      "${MATLAB_LIB_DIR}/libmat.lib"
      "${MATLAB_LIB_DIR}/mclmcr.lib"
      "${MATLAB_LIB_DIR}/libmx.lib"
      "${MATLAB_LIB_DIR}/mclmcrrt.lib"
  )
  if("${MATLAB_TF_ALGORITHM_PACK_PATH}" STREQUAL "")
  else()
    message("Build TF Algorithm Pack")
    include_directories("${MATLAB_TF_ALGORITHM_PACK_PATH}")
    set(
      MATLAB_TF_ALGORITHM_PACK_LIBS
        ${MATLAB_DLL_LIBS}
        "${MATLAB_TF_ALGORITHM_PACK_PATH}/generate_table_8.lib"
        "${MATLAB_TF_ALGORITHM_PACK_PATH}/test.lib"
    )
    set(
      MATLAB_TF_ALGORITHM_PACK_SRC
        tf_algorithm_pack.cpp
        tf_algorithm_pack.h
        mini_generate_table_8.cpp
        mini_generate_table_8.h
        rte_cart_server.cpp
        rte_cart_server.h
        rte_cart_widget.cpp
        rte_cart_widget.h
        rte_cart_widget.ui
        rte_cart_server_impl.cpp
        rte_cart_server_impl.h
    )
  endif()
endif()

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
      distance_over_time_chart_with_threshold.h
      distance_over_time_chart_with_threshold.cpp
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
      range_detection_page.cpp
      range_detection_page.h
      serial_number_page.cpp
      serial_number_page.h
      release_mode_page.cpp
      release_mode_page.h
      static_unique_ptr_cast.h
      save_log_widget.cpp
      save_log_widget.h
      save_log_widget.ui
      driver_base.cpp
      driver_base.h
      tf02_driver.cpp
      tf02_driver.h
      cart_driver.cpp
      cart_driver.h
      cart_test_widget.cpp
      cart_test_widget.h
      cart_test_widget.ui
      cart_test_sheet_widget.cpp
      cart_test_sheet_widget.h
      cart_test_sheet_widget.ui
      cart_test_sheet.cpp
      cart_test_sheet.h
      connection_widget.cpp
      connection_widget.h
      connection_widget.ui
      measure_display_widget.cpp
      measure_display_widget.h
      measure_display_widget.ui
      custom_widget.cpp
      custom_widget.h
      tfmini_driver_parser.cpp
      tfmini_driver_command.cpp
      tfmini_driver.cpp
      tfmini_driver.h
      tfmini_command_widgets.cpp
      tfmini_command_widgets.h
      common_command_widget.cpp
      common_command_widget.h
      common_tfmini_widget.cpp
      common_tfmini_widget.h
      tfmini_driver_server.cpp
      tfmini_driver_server.h
      command_block.cpp
      command_block.h
      command_block.ui
      tfmini_plot_widget.cpp
      tfmini_plot_widget.h
      tfmini_plot_widget.ui
      driver_server.cpp
      driver_server.h
      mini_rte_cart.cpp
      mini_rte_cart.h
      custom_object.cpp
      custom_object.h
      mini_table_burner.cpp
      mini_table_burner.h
      mini_table_burner_base.cpp
      mini_table_burner_base.h
      tfmini_driver_impl.cpp
      tfmini_driver_impl.h
      ${MATLAB_TF_ALGORITHM_PACK_SRC}
)

target_link_libraries(
  tf03_common
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Charts
    ${MATLAB_TF_ALGORITHM_PACK_LIBS}
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
