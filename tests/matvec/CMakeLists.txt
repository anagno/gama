# ------------------------------------------------------------------------
#
# matvec-basic
#

add_executable(matvec-basic matvec-basic.cpp)

target_link_libraries(matvec-basic libgama)

add_test(NAME matvec-basic COMMAND matvec-basic)

# ------------------------------------------------------------------------
#
# matvec_demo_001
#

add_executable(matvec_demo_001 matvec_demo_001.cpp)

target_link_libraries(matvec_demo_001 libgama)

add_test(NAME matvec_demo_001 COMMAND matvec_demo_001)

# ------------------------------------------------------------------------
#
# matvec_demo_002
#

add_executable(matvec_demo_002 matvec_demo_002.cpp)

target_link_libraries(matvec_demo_002 libgama)

add_test(NAME matvec_demo_002 COMMAND matvec_demo_002)

# ------------------------------------------------------------------------
#
# matvec_demo_003
#

add_executable(matvec_demo_003 matvec_demo_003.cpp)

target_link_libraries(matvec_demo_003 libgama)

add_test(NAME matvec_demo_003 COMMAND matvec_demo_003)

# ------------------------------------------------------------------------
#
# matvec_demo_004
#

add_executable(matvec_demo_004 matvec_demo_004.cpp)

target_link_libraries(matvec_demo_004 libgama)

add_test(NAME matvec_demo_004 COMMAND matvec_demo_004)

# ------------------------------------------------------------------------
#
# matvec_demo_005
#

add_executable(matvec_demo_005 matvec_demo_005.cpp)

target_link_libraries(matvec_demo_005 libgama)

add_test(NAME matvec_demo_005 COMMAND matvec_demo_005)

# ------------------------------------------------------------------------
#
# matvec_demo_006
#

add_executable(matvec_demo_006 matvec_demo_006.cpp)

target_link_libraries(matvec_demo_006 libgama)

add_test(NAME matvec_demo_006 COMMAND matvec_demo_006)

# ------------------------------------------------------------------------
#
# matvec_test_001
#

add_executable(matvec_test_001 matvec_test_001.cpp)

target_link_libraries(matvec_test_001 libgama)

add_test(NAME matvec_test_001 COMMAND matvec_test_001)

# ------------------------------------------------------------------------
#
# matvec_test_002
#

add_executable(matvec_test_002 matvec_test_002.cpp)

target_link_libraries(matvec_test_002 libgama)

add_test(NAME matvec_test_002 COMMAND matvec_test_002)

# ------------------------------------------------------------------------
#
# matvec_test_003
#

add_executable(matvec_test_003 matvec_test_003.cpp)

target_link_libraries(matvec_test_003 libgama)

add_test(NAME matvec_test_003 COMMAND matvec_test_003)

# ------------------------------------------------------------------------
#
# matvec_test_004
#

add_executable(matvec_test_004 matvec_test_004.cpp)

target_link_libraries(matvec_test_004 libgama)

add_test(NAME matvec_test_004 COMMAND matvec_test_004)

# ------------------------------------------------------------------------
#
# matvec_test_005
#

add_executable(matvec_test_005 matvec_test_005.cpp)

target_link_libraries(matvec_test_005 libgama)

add_test(NAME matvec_test_005 COMMAND matvec_test_005)

# ------------------------------------------------------------------------
#
# simple-inversion
#

add_executable(simple-inversion simple-inversion.cpp)

target_link_libraries(simple-inversion libgama)

add_test(NAME simple-inversion COMMAND simple-inversion)

# ------------------------------------------------------------------------
#
# sparse-demo
#

add_executable(sparse-demo sparse-demo.cpp)

target_link_libraries(sparse-demo libgama)

add_test(NAME sparse-demo COMMAND sparse-demo)
