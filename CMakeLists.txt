cmake_minimum_required(VERSION 3.16)

project (gnu-gama VERSION 2.9.4)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

# Tests
# https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Testing-With-CTest#dynamic-analysis
option(BUILD_TESTING "" OFF)
include (CTest)

include(CMakeDependentOption)
option(USE_SQLITE3 "Use the SQLite 3 on the compilation of the source code" OFF)

if(BUILD_TESTING)
  cmake_dependent_option(USE_OCTAVE
                         "Use the liboctave-dev on the compilation of the source code"
                         OFF
                         "BUILD_TESTING"
                         OFF)
endif()

if(NOT CONAN_EXPORTED)

  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(
      STATUS
        "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
      DOWNLOAD
      "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.15/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake")
  endif()

  set(OPTIONAL_CONAN_PARAMETERS OPTIONS)

  if(USE_SQLITE3)
    set(OPTIONAL_CONAN_PARAMETERS ${OPTIONAL_CONAN_PARAMETERS} sqlite3=True)
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)
  conan_check(VERSION 1.19.1 REQUIRED)

  conan_cmake_run(CONANFILE
                  conanfile.py
                  BASIC_SETUP
                  CMAKE_TARGETS
                  BUILD
                  missing
                  ${OPTIONAL_CONAN_PARAMETERS})

  set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
  set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

endif()

if(USE_SQLITE3)
  find_package(SQLite3 3.20 REQUIRED)
endif()

if(BUILD_TESTING)
  find_package(LibXml2 2.9 REQUIRED)
  find_program(LIBXML2_XMLLINT_EXECUTABLE xmllint REQUIRED)

  if(USE_OCTAVE)
    #sudo apt-get install liboctave-dev
    find_package(Octave 4.2 REQUIRED)
  endif()
endif()

if(SQLITE3_FOUND)
  add_definitions(-DGNU_GAMA_LOCAL_SQLITE_READER)
endif()

# The programs located under Parsing/Application are only needed during the build 
# of the library. These programs generate some source code, which is necessary 
# for the compilation of the library. So we have to first build them, generate 
# the code and then build the rest of the library.
# When cross-compiling the project (e.g. for wasm), we do not really want
# to cross-compile these programs. What we really want is just to build them 
# and generate the necessary headers. 
# So we will also compile them with the native compiler to generate the headers.

# Some resources: 
# * https://stackoverflow.com/questions/36084785/building-a-tool-immediately-so-it-can-be-used-later-in-same-cmake-run
# * https://cmake.org/pipermail/cmake/2010-January/034857.html

if(NOT CMAKE_CROSSCOMPILING) 
  set(BUILD_TOOS_CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})
  set(BUILD_TOOS_CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
else()
  if(NOT DEFINED BUILD_TOOS_CMAKE_CXX_COMPILER)
    message(FATAL_ERROR 
      " BUILD_TOOS_CMAKE_CXX_COMPILER is not defined.  You must tell CMake which compiler to use."
      "We also need a native compiler for compiling some build tools for generating headers.") 
  endif()
  set(BUILD_TOOS_CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/BuildTools)
  set(conan_configure_command conan install ${CMAKE_CURRENT_SOURCE_DIR}/Parsing --install-folder ${CMAKE_CURRENT_BINARY_DIR}/BuildTools/)
endif()


include(ExternalProject)
ExternalProject_Add(BuildTools
  PREFIX ${CMAKE_BINARY_DIR}/BuildTools
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/Parsing
  BINARY_DIR ${CMAKE_BINARY_DIR}/BuildTools
  INSTALL_DIR ${CMAKE_BINARY_DIR}/BuildTools
  # I am overloading the functionality of the UPDATE_COMMAND to execute conan
  DOWNLOAD_COMMAND ${conan_configure_command}
  CMAKE_ARGS
    -DCMAKE_CXX_COMPILER=${BUILD_TOOS_CMAKE_CXX_COMPILER}
    -DCMAKE_MODULE_PATH=${BUILD_TOOS_CMAKE_MODULE_PATH}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_property(BuildTools BINARY_DIR)
if(WIN32)
  set(BuildTools_ellipsoids_xml ${BINARY_DIR}/libexec/ellipsoids_xml.exe)
  set(BuildTools_slovnikar ${BINARY_DIR}/libexec/slovnikar.exe)
else()
  set(BuildTools_ellipsoids_xml ${BINARY_DIR}/libexec/ellipsoids_xml)
  set(BuildTools_slovnikar ${BINARY_DIR}/libexec/slovnikar)
endif()


add_subdirectory(Math)
add_subdirectory(Utilities)
add_subdirectory(Parsing)

add_subdirectory(lib)
add_subdirectory(LocalNetwork)
add_subdirectory(GlobalNetwork)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
