set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
find_package(Qt5SerialPort REQUIRED)
# find_package(Qt5Charts)

if (SUPPORT_DEVEL_MODE_PROTOCOL)
  add_definitions(-DSUPPORT_DEVEL_MODE_PROTOCOL_)
endif()

add_executable(
  tf03_setup
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
)

target_link_libraries(
  tf03_setup
    Qt5::Widgets
    Qt5::SerialPort
)

install(TARGETS tf03_setup
  # IMPORTANT: Add the tf03_setup executable to the "export-set"
  EXPORT tf03_sdkTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
