cmake_minimum_required(VERSION 3.16)
project (Utilities VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)

add_subdirectory(Business)
add_subdirectory(Service)
