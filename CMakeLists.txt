cmake_minimum_required(VERSION 3.10)

project (gnu-gama VERSION 2.7)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_CXX_STANDARD 11)

# Tests
# https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Testing-With-CTest#dynamic-analysis
include (CTest)

option(USE_CONAN "Use conan package manager for the installation of the dependencies" OFF)

if(USE_CONAN)
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

  include(CMakeDependentOption)
  set(OPTIONAL_CONAN_PARAMETERS OPTIONS)

  CMAKE_DEPENDENT_OPTION(INSTALL_SQLITE3 "Install the sqlite3 plugin" OFF "USE_CONAN" OFF)
  if(INSTALL_SQLITE3)
    set(OPTIONAL_CONAN_PARAMETERS ${OPTIONAL_CONAN_PARAMETERS} sqlite3=True)
  endif()

  if(BUILD_TESTING)
    CMAKE_DEPENDENT_OPTION(INSTALL_LIBXML2 "Install the libxml2" OFF "USE_CONAN" OFF)
    if(INSTALL_LIBXML2)
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

find_package(EXPAT 2.2 REQUIRED)
find_package(SQLite3 3.20 QUIET)

if(BUILD_TESTING)
  find_package(LibXml2 2.9 QUIET)

  #sudo apt-get install liboctave-dev
  find_package(Octave 4.2 QUIET)
endif()

if(SQLITE3_FOUND)
  add_definitions(-DGNU_GAMA_LOCAL_SQLITE_READER)
endif()

add_subdirectory(scripts)
add_subdirectory(lib)
add_subdirectory(bin)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
