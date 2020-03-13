cmake_minimum_required(VERSION 3.10)

project (gnu-gama VERSION 2.7.2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_CXX_STANDARD 14)
add_definitions(-DGNU_gama_expat_1_1)


# Tests
# https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Testing-With-CTest#dynamic-analysis
option(BUILD_TESTING "" OFF)
include (CTest)

include(CMakeDependentOption)
option(USE_SQLITE3 "Use the SQLite 3 on the compilation of the source code" OFF)

if(BUILD_TESTING)
  cmake_dependent_option(USE_LIBXML2
                         "Use the libxml2 on the compilation of the source code"
                         OFF
                         "BUILD_TESTING"
                         OFF)

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
      "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.14/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake")
  endif()

  set(OPTIONAL_CONAN_PARAMETERS OPTIONS)

  if(USE_SQLITE3)
    set(OPTIONAL_CONAN_PARAMETERS ${OPTIONAL_CONAN_PARAMETERS} sqlite3=True)
  endif()

  if(BUILD_TESTING)
    if(USE_LIBXML2)
      set(OPTIONAL_CONAN_PARAMETERS ${OPTIONAL_CONAN_PARAMETERS} libxml2=True)
    endif()
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

endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Expat 2.2 REQUIRED)

if(USE_SQLITE3)
  find_package(SQLite3 3.20 REQUIRED)
endif()

if(BUILD_TESTING)
  if(USE_LIBXML2)
    find_package(LibXml2 2.9 REQUIRED)
  endif()
  

  if(USE_OCTAVE)
    #sudo apt-get install liboctave-dev
    find_package(Octave 4.2 REQUIRED)
  endif()
endif()

if(SQLITE3_FOUND)
  add_definitions(-DGNU_GAMA_LOCAL_SQLITE_READER)
endif()

set(conan_configure_command conan install ${CMAKE_CURRENT_SOURCE_DIR}/scripts --install-folder ${CMAKE_CURRENT_BINARY_DIR}/scripts/)

if(NOT CMAKE_CROSSCOMPILING)
  set(BUILD_TOOS_CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})
else()
  if(NOT DEFINED BUILD_TOOS_CMAKE_CXX_COMPILER)
    message(FATAL_ERROR 
      " BUILD_TOOS_CMAKE_CXX_COMPILER is not defined.  You must tell CMake which compiler to use."
      "We also need a native compiler for compiling some build tools for generating headers.") 
  endif()
endif()

include(ExternalProject)
ExternalProject_Add(BuildTools
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/scripts
  EXCLUDE_FROM_ALL
  # I am overloading the functionality of the UPDATE_COMMAND to execute conan
  UPDATE_COMMAND ${conan_configure_command}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_CXX_COMPILER=${BUILD_TOOS_CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
    -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
    -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
  INSTALL_COMMAND "")

ExternalProject_Get_property(BuildTools BINARY_DIR)
set(BuildTools_BUILD_DIR ${BINARY_DIR})


add_subdirectory(lib)
add_subdirectory(bin)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
