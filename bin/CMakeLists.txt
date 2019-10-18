# Binaries
add_executable(gama-local gama-local.cpp)

target_link_libraries(gama-local libgama)

add_executable(gama-g3 gama-g3.cpp)

target_link_libraries(gama-g3 libgama)

# Installation
install(TARGETS gama-local gama-g3 DESTINATION bin)
