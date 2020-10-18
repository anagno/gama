add_executable(gama-g3 gama-g3.cpp)

target_link_libraries(gama-g3 GaMa::libgama)

install(
    TARGETS gama-g3
    DESTINATION ${CMAKE_INSTALL_BINDIR})