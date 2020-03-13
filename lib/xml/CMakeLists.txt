include(CMake_xml_SrcFiles.cmake)

add_library(xml ${source_files})

target_include_directories(xml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(xml PUBLIC Expat::Expat)
