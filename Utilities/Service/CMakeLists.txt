include(CMake_utilities_SrcFiles.cmake)

add_library(Service_Utilities ${source_files})

add_library(GaMa::Service_Utilities ALIAS Service_Utilities)

set_target_properties(Service_Utilities
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Service_Utilities
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(GenerateExportHeader)
generate_export_header(Service_Utilities
                       EXPORT_FILE_NAME
                       UtilitiesDLL.h
                       EXPORT_MACRO_NAME
                       UtilitiesAPI)