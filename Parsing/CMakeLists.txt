cmake_minimum_required(VERSION 3.10)
project (Parsing VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(EXPAT 2.2 REQUIRED)

include(GenerateExportHeader)
include(GNUInstallDirs)

add_subdirectory(Business)
add_subdirectory(Service)
add_subdirectory(Application)

