# These programs are only needed during the build of the library.
# When cross-compiling the project (e.g. for wasm), we do not really want
# to cross-compile these programs. What we really want is just to build them 
# and generate the necessary headers. 
# So we create a small a project out of them, that can be used as an external
# project in the main build.

# Some resources: 
# * https://stackoverflow.com/questions/36084785/building-a-tool-immediately-so-it-can-be-used-later-in-same-cmake-run
# * https://cmake.org/pipermail/cmake/2010-January/034857.html

cmake_minimum_required(VERSION 3.10)
project (build-tools)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(Expat 2.2 REQUIRED)

# Really not the best!!! I will have to rethink it!!!
add_subdirectory(../lib/xml xml)
add_executable(slovnikar slovnikar.cpp)

target_link_libraries(slovnikar PRIVATE xml)

add_executable(ellipsoids_xml ellipsoids_xml.cpp)

target_link_libraries(ellipsoids_xml PRIVATE xml)

install(TARGETS ellipsoids_xml slovnikar DESTINATION bin)
