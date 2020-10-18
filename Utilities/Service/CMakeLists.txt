include(CMake_utilities_SrcFiles.cmake)

add_library(Service_Utilities ${source_files})

add_library(GaMa::Service_Utilities ALIAS Service_Utilities)

set_target_properties(Service_Utilities
  PROPERTIES VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR})

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

set_property(TARGET Service_Utilities
  PROPERTY PUBLIC_HEADER
  ${header_files} ${CMAKE_CURRENT_BINARY_DIR}/UtilitiesDLL.h)


install(
    TARGETS Service_Utilities
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/${PROJECT_NAME}/Service/)