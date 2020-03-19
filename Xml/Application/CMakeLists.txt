add_executable(slovnikar slovnikar.cpp)

target_link_libraries(slovnikar PRIVATE GaMa::Service_Xml)

add_executable(ellipsoids_xml ellipsoids_xml.cpp)

target_link_libraries(ellipsoids_xml PRIVATE GaMa::Service_Xml)

install(TARGETS ellipsoids_xml slovnikar DESTINATION bin)
