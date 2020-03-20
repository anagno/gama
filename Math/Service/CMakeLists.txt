include(CMake_math_SrcFiles.cmake)

add_library(Service_Math INTERFACE)

add_library(GaMa::Service_Math ALIAS Service_Math)

target_sources(Service_Math INTERFACE)

target_include_directories(
  Service_Math
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
            $<INSTALL_INTERFACE:GaMa/${PROJECT_NAME}>)
