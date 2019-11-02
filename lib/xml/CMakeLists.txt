include(CMake_xml_SrcFiles.cmake)

add_library(xml ${source_files})

target_include_directories(xml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(USE_CONAN)
  target_link_libraries(xml PUBLIC Expat::Expat)
else()
  target_link_libraries(xml PUBLIC EXPAT::EXPAT)
endif()
