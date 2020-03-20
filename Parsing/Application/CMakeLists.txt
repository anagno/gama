add_executable(slovnikar slovnikar.cpp)

target_link_libraries(slovnikar PRIVATE GaMa::Service_Parsing)

add_executable(ellipsoids_xml ellipsoids_xml.cpp)

target_link_libraries(ellipsoids_xml PRIVATE GaMa::Service_Parsing)

install(TARGETS ellipsoids_xml slovnikar DESTINATION bin)
