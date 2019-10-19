# Binaries
add_executable(slovnikar slovnikar.cpp)

target_include_directories(slovnikar PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(slovnikar PRIVATE xml)

add_executable(ellipsoids_xml ellipsoids_xml.cpp)

target_include_directories(ellipsoids_xml PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(ellipsoids_xml PRIVATE xml)