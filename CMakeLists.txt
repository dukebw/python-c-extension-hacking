cmake_minimum_required(VERSION 3.24)
project(CPythonHacking VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Common installation directories
include(GNUInstallDirs)
# TODO: @loader_path on MacOS, $ORIGIN on Linux
set(CMAKE_INSTALL_RPATH @loader_path)

# Use -fPIC even if statically compiled
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =======
# vector_add library
# =======

add_library(vector_add
  src/vector_add.h
  src/vector_add.cpp)

# =======
# fib library
# =======

# Find Python3 for Python module development
find_package(Python3 COMPONENTS Interpreter Development.Module NumPy REQUIRED)

# Add fib extension module library as build target
Python3_add_library(fib src/fib.c)

target_link_libraries(fib PRIVATE vector_add Python3::NumPy)

# Ask distutils what the Python extension module suffix is
# NOTE: this is based on pybind11's code in pybind11NewTools.cmake
execute_process(
  COMMAND
    "python" "-c"
    "import sys, importlib; s = importlib.import_module('distutils.sysconfig' if sys.version_info < (3, 10) else 'sysconfig'); print(s.get_config_var('EXT_SUFFIX') or s.get_config_var('SO'))"
  OUTPUT_VARIABLE _PYTHON_MODULE_EXTENSION
  ERROR_VARIABLE _PYTHON_MODULE_EXTENSION_ERR
  OUTPUT_STRIP_TRAILING_WHITESPACE)

if(_PYTHON_MODULE_EXTENSION STREQUAL "")
  message(
    FATAL_ERROR "could not query the module file extension. "
                "Full error message:\n${_PYTHON_MODULE_EXTENSION_ERR}")
endif()

set_target_properties(
  fib
  PROPERTIES
  # Set the Python extension module name, prefix, and suffix
  OUTPUT_NAME "fib"
  PREFIX ""
  SUFFIX "${_PYTHON_MODULE_EXTENSION}"
  INTERPROCEDURAL_OPTIMIZATION ON
  C_VISIBILITY_PRESET hidden
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON)

# Install the Python module and its dependent dynamic library
install(
  TARGETS fib vector_add
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
