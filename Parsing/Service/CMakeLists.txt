include(CMake_xml_SrcFiles.cmake)

add_library(Service_Parsing ${source_files})

add_library(GaMa::Service_Parsing ALIAS Service_Parsing)

set_target_properties(Service_Parsing
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Service_Parsing
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Service_Parsing PUBLIC EXPAT::EXPAT)

generate_export_header(Service_Parsing
                       EXPORT_FILE_NAME
                       ParsingServiceDLL.h
                       EXPORT_MACRO_NAME
                       ParsingServiceAPI)

set_property(TARGET Service_Parsing
  PROPERTY PUBLIC_HEADER
  ${header_files} ${CMAKE_CURRENT_BINARY_DIR}/ParsingServiceDLL.h)

install(
    TARGETS Service_Parsing
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/${PROJECT_NAME}/Service/)
