# Binaries
add_executable(slovnikar slovnikar.cpp)

target_link_libraries(slovnikar libgama)

add_executable(
  ellipsoids_xml
  ellipsoids_xml.cpp)

target_include_directories(ellipsoids_xml PUBLIC ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(ellipsoids_xml PRIVATE xml)