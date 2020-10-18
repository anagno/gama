include(CMake_parsing_SrcFiles.cmake)

add_library(Business_Parsing ${source_files})

add_library(GaMa::Business_Parsing ALIAS Business_Parsing)

set_target_properties(Business_Parsing
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Business_Parsing
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Business_Parsing 
  PUBLIC GaMa::Service_Parsing)

generate_export_header(Business_Parsing
                       EXPORT_FILE_NAME
                       ParsingDLL.h
                       EXPORT_MACRO_NAME
                       ParsingAPI)

set_property(TARGET Business_Parsing
  PROPERTY PUBLIC_HEADER
  ${header_files} ${CMAKE_CURRENT_BINARY_DIR}/ParsingDLL.h)

install(
    TARGETS Business_Parsing
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/${PROJECT_NAME}/Business/)
