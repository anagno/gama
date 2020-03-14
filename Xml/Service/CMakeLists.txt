include(CMake_xml_SrcFiles.cmake)

add_library(Service_Xml SHARED ${source_files})

add_library(GaMa::Service_Xml ALIAS Service_Xml)

set_target_properties(Service_Xml
                      PROPERTIES VERSION 
                                 0.1.0
                                 SOVERSION
                                 0.1)

target_include_directories(
  Service_Xml
  PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/GaMa>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/impl)

target_include_directories(Service_Xml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Service_Xml PUBLIC Expat::Expat)
