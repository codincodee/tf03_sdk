set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)

if(DEVEL_DEBUG)
  add_definitions(-DDEVEL_DEBUG)
endif()

add_executable(
  tfmini_test
  WIN32
    main.cpp
    main_window.cpp
    main_window.h
    main_window.ui
    resource.qrc
)

target_link_libraries(
  tfmini_test
    Qt5::WinMain
    tf03_common
)

install(TARGETS tfmini_test
  # IMPORTANT: Add the tfmini_test executable to the "export-set"
  EXPORT tf03_sdkTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
