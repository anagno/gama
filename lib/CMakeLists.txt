# add_definitions(-DDEBUG_ACORD2) add_definitions(-DA2G_DEBUG)
include(CMake_libGama_SrcFiles.cmake)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama)

add_custom_command(OUTPUT gnu_gama/ellipsoids.cpp
                          gnu_gama/ellipsoids.h
                          gnu_gama/ellipsoids.html
                          gnu_gama/ellipsoids.texi
                   DEPENDS BuildTools
                   COMMAND ${BuildTools_ellipsoids_xml}
                           ${Parsing_SOURCE_DIR}/Schemas/ellipsoids.xml
                           ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama)

# file(GLOB_RECURSE) commands is being misused here
# Normally his should be avoided because if files are added or removed, 
# CMake is not automatically re-run, so the build is unaware of the change.
# https://cmake.org/cmake/help/v3.10/command/file.html
file(GLOB_RECURSE language_files
  ${Parsing_SOURCE_DIR}/Schemas/lang/*/*.lang
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local)

add_custom_command(OUTPUT gnu_gama/local/language.h
                          gnu_gama/local/language.cpp
                   DEPENDS BuildTools
                   COMMAND ${BuildTools_slovnikar} ${language_files}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local)

add_library(libgama ${source_files}
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/ellipsoids.cpp
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/ellipsoids.h
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local/language.h
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local/language.cpp
                    )

add_library(GaMa::libgama ALIAS libgama)

target_include_directories(libgama
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                  ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(libgama 
  PUBLIC 
    GaMa::Service_Parsing 
    GaMa::Business_Parsing 
    GaMa::Service_Math 
    GaMa::Business_MathCore 
    GaMa::Business_MathAdjustment
    GaMa::Service_Utilities
    GaMa::Business_Utilities)

include(GenerateExportHeader)
generate_export_header(libgama
                       EXPORT_FILE_NAME
                       ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/GaMaDLL.h
                       EXPORT_MACRO_NAME
                       GaMaAPI)

if(SQLITE3_FOUND)
  if(USE_SQLITE3)
    target_link_libraries(libgama PRIVATE SQLite::SQLite3)
  endif()
endif()


set_property(TARGET libgama
  PROPERTY PUBLIC_HEADER
  #${header_files}
  ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/GaMaDLL.h)


install(
    TARGETS libgama
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/gnu_gama/
    )

# Workaround to keep the organization of the files, because I am bore to fix it better
file(GLOB DEPLOY_FILES_AND_DIRS "${PROJECT_SOURCE_DIR}/lib/gnu_gama/*")
foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
   IF( IS_DIRECTORY "${ITEM}" )
      LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
   ELSE()
      LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
   ENDIF()
endforeach()
INSTALL( FILES ${FILES_TO_DEPLOY} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/gnu_gama/ )
INSTALL( DIRECTORY ${DIRS_TO_DEPLOY} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GaMa/gnu_gama/ )
