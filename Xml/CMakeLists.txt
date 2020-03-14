cmake_minimum_required(VERSION 3.10)
project (Xml)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(Expat 2.2 REQUIRED)

add_subdirectory(Service)

if(NOT CMAKE_CROSSCOMPILING) 
  add_subdirectory(Application)
endif()
