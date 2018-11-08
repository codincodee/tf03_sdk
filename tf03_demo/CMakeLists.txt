set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)

add_executable(
  tf03_demo
  WIN32
    main.cpp
    main_window.cpp
    main_window.h
    main_window.ui
    resource.qrc
)

target_link_libraries(
  tf03_demo
    Qt5::WinMain
    tf03_common
)

install(TARGETS tf03_demo
  # IMPORTANT: Add the tf03_demo executable to the "export-set"
  EXPORT tf03_sdkTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
