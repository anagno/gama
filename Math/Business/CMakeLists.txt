include(CMake_math_SrcFiles.cmake)

add_library(Business_Math ${source_files})

add_library(GaMa::Business_Math ALIAS Business_Math)

target_sources(Business_Math INTERFACE ${source_files})

set_target_properties(Business_Math
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Business_Math
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(GenerateExportHeader)
generate_export_header(Business_Math
                       EXPORT_FILE_NAME
                       MathDLL.h
                       EXPORT_MACRO_NAME
                       MathAPI)
