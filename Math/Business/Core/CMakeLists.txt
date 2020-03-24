include(CMake_math_SrcFiles.cmake)

add_library(Business_MathCore ${source_files})

add_library(GaMa::Business_MathCore ALIAS Business_MathCore)

target_sources(Business_MathCore INTERFACE ${source_files})

set_target_properties(Business_MathCore
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Business_MathCore
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(GenerateExportHeader)
generate_export_header(Business_MathCore
                       EXPORT_FILE_NAME
                       MathDLL.h
                       EXPORT_MACRO_NAME
                       MathAPI)
