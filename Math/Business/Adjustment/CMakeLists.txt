include(CMake_adjustment_SrcFiles.cmake)

add_library(Business_MathAdjustment ${source_files})

add_library(GaMa::Business_MathAdjustment ALIAS Business_MathAdjustment)

target_sources(Business_MathAdjustment INTERFACE ${source_files})

set_target_properties(Business_MathAdjustment
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Business_MathAdjustment
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Business_MathAdjustment 
  PUBLIC GaMa::Service_Math
  PRIVATE GaMa::Service_Utilities) # Unwanted dependency, but for the moment we keep it
                                   # This dependency does not allow the Math project 
                                   # to compile by itself


include(GenerateExportHeader)
generate_export_header(Business_MathAdjustment
                       EXPORT_FILE_NAME
                       AdjustmentDLL.h
                       EXPORT_MACRO_NAME
                       AdjustmentAPI)
