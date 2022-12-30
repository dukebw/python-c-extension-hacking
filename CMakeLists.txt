cmake_minimum_required(VERSION 3.24)
project(CPythonHacking VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common installation directories
include(GNUInstallDirs)

# Use -fPIC even if statically compiled
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =======
# fib library
# =======

# Find Python3 for Python module development
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

Python3_add_library(fib src/fib.c)
add_library(Fib::fib ALIAS fib)

# =======
# Install
# =======

# See official documentation on exporting targets:
# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html#exporting-targets

# Install the target with C++ code
install(
    TARGETS fib
    EXPORT FibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install the Python module
install(
    TARGETS fib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/..
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/..
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/..)

# Install the exported targets
install(
  EXPORT FibTargets
  FILE FibTargets.cmake
  NAMESPACE Fib::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Fib)

# Create a CMake package
include(CMakePackageConfigHelpers)

# Prepare the Config.cmake.in content
set(PACKAGE_INIT_MACRO "@PACKAGE_INIT@")
set(CONFIG_CMAKE_IN "\
@PACKAGE_INIT_MACRO@\n\
include(\"\${CMAKE_CURRENT_LIST_DIR}/FibTargets.cmake\")\n\
check_required_components(Fib)\n"
)

# Create Config.cmake.in
file(CONFIGURE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Config.cmake.in
    CONTENT ${CONFIG_CMAKE_IN}
    @ONLY)

# Create FibConfig.cmake
configure_package_config_file(
  ${CMAKE_CURRENT_BINARY_DIR}/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/FibConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Fib)

# Create FibConfigVersion.cmake
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/FibConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)

# Install CMake package files
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/FibConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/FibConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Fib
)

# ===============
# Python bindings
# ===============

set(FIB_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
