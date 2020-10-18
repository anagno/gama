cmake_minimum_required(VERSION 3.16)
project (Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CONAN_EXPORTED)
    include(CMakeDependentOption)
    find_package(GaMa REQUIRED)
    enable_testing()

    find_program(gama-local gama-local REQUIRED)
    find_program(gama-g3 gama-g3 REQUIRED)
    find_program(gama-local-xml2sql gama-local-xml2sql REQUIRED)
    find_program(gama-local-xml2txt gama-local-xml2txt REQUIRED)

    set(GAMA_XML "${GaMa_libgama_RES_DIRS}/Schemas")

    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)

else()
    set(GAMA_XML "${Parsing_SOURCE_DIR}/Schemas")
endif()

find_program(LIBXML2_XMLLINT_EXECUTABLE xmllint REQUIRED)

cmake_dependent_option(USE_OCTAVE
    "Use the liboctave-dev on the compilation of the source code" OFF
    "BUILD_TESTING" OFF)

if(USE_OCTAVE)
    #sudo apt-get install liboctave-dev
    find_package(Octave 4.2 REQUIRED)
endif()



add_subdirectory(acord2)
add_subdirectory(gama-g3)
add_subdirectory(gama-local)
add_subdirectory(matvec)
add_subdirectory(statan)

