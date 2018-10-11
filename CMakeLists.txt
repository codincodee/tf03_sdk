cmake_minimum_required(VERSION 2.8)
project(tf03_sdk C CXX)

set(CMAKE_CXX_STANDARD 14)

set(tf03_sdk_MAJOR_VERSION 1)
set(tf03_sdk_MINOR_VERSION 2)
set(tf03_sdk_PATCH_VERSION 5)
set(tf03_sdk_VERSION
  ${tf03_sdk_MAJOR_VERSION}.${tf03_sdk_MINOR_VERSION}.${tf03_sdk_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/tf03_sdk)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"   # to find tf0x_drivers/tf0x_drivers.h
  "${PROJECT_BINARY_DIR}")  # to find tf0x_drivers/config.h

# Add sub-directories
add_subdirectory(tf03_setup)

# The interesting stuff goes here
# ===============================

# Add all targets to the build-tree export set
export(
  TARGETS
    tf03_setup
  FILE
    "${PROJECT_BINARY_DIR}/tf03_sdkTargets.cmake"
)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE tf03_sdk)

# Create the tf03_sdkConfig.cmake and tf0x_devConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(tf03_sdkConfig.cmake.in
  "${PROJECT_BINARY_DIR}/tf03_sdkConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${tf03_sdk_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(tf03_sdkConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tf03_sdkConfig.cmake" @ONLY)
# ... for both
configure_file(tf03_sdkConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/tf03_sdkConfigVersion.cmake" @ONLY)

# Install the tf03_sdkConfig.cmake and tf03_sdkConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tf03_sdkConfig.cmake"
  "${PROJECT_BINARY_DIR}/tf03_sdkConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT tf03_sdkTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
