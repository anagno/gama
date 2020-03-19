# Binaries
add_executable(gama-local gama-local.cpp)

target_link_libraries(gama-local libgama)

if(EMSCRIPTEN)
  #set_target_properties(gama-local PROPERTIES
  #  SUFFIX ".html"
  #  LINK_FLAGS "--emrun")

  #set_target_properties(gama-local PROPERTIES LINK_FLAGS "-s BUILD_AS_SHARED_LIB=1 -s SIDE_MODULE=1 -s EXPORT_ALL=1 -s DISABLE_EXCEPTION_CATCHING=0 -s FORCE_FILESYSTEM=1 -s SINGLE_FILE=1 -s BINARYEN_ASYNC_COMPILATION=0")
  set_target_properties(gama-local PROPERTIES 
    LINK_FLAGS 
    "-s DEMANGLE_SUPPORT=1 -s MAIN_MODULE=1 -s EXPORT_ALL=1 -s DISABLE_EXCEPTION_CATCHING=0 -s FORCE_FILESYSTEM=1 -s SINGLE_FILE=1 -s BINARYEN_ASYNC_COMPILATION=0 --embed-file ${CMAKE_SOURCE_DIR}/tests/gama-local/input")

endif()

add_executable(gama-g3 gama-g3.cpp)

target_link_libraries(gama-g3 libgama)

# Installation
install(TARGETS gama-local gama-g3 DESTINATION bin)

add_executable(gama-local-xml2sql gama-local-xml2sql.cpp)

target_link_libraries(gama-local-xml2sql PRIVATE libgama)

add_executable(gama-local-xml2txt gama-local-xml2txt.cpp)

target_link_libraries(gama-local-xml2txt PRIVATE libgama)