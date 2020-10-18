include(CMake_utilities_SrcFiles.cmake)

# https://crascit.com/2017/04/18/generated-sources-in-cmake-builds/
# https://cmake.org/cmake/help/v3.10/variable/CMAKE_VERSION.html
configure_file(src/version.cpp.in src/version.cpp @ONLY)

add_library(Business_Utilities 
    ${source_files} 
    ${CMAKE_CURRENT_BINARY_DIR}/src/version.cpp)

add_library(GaMa::Business_Utilities ALIAS Business_Utilities)

set_target_properties(Business_Utilities
    PROPERTIES 
        VERSION 0.1.0
        SOVERSION 0.1)

target_include_directories(Business_Utilities
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

generate_export_header(Business_Utilities
    EXPORT_FILE_NAME UtilitiesBusinessDLL.h
    EXPORT_MACRO_NAME UtilitiesBusinessAPI)

set_property(TARGET Business_Utilities
  PROPERTY PUBLIC_HEADER
  ${header_files} ${CMAKE_CURRENT_BINARY_DIR}/UtilitiesBusinessDLL.h)

install(
    TARGETS Business_Utilities
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/${PROJECT_NAME}/Business/)