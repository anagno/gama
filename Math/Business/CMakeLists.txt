include(CMake_sparse_SrcFiles.cmake)

add_library(Business_Math INTERFACE)

add_library(GaMa::Business_Math ALIAS Business_Math)

target_sources(Business_Math INTERFACE ${source_files})

target_include_directories(
  Business_Math
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
            $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>)
