# ------------------------------------------------------------------------
#
# chi2_critical_values
#

add_executable(chi2_critical_values chi2_critical_values.cpp)

target_link_libraries(chi2_critical_values libgama)

add_test(NAME chi2_critical_values COMMAND chi2_critical_values)


# ------------------------------------------------------------------------
#
# kolmogorov_smirnov_cdf
#

add_executable(kolmogorov_smirnov_cdf kolmogorov_smirnov_cdf.cpp)

target_link_libraries(kolmogorov_smirnov_cdf libgama)

add_test(NAME kolmogorov_smirnov_cdf COMMAND kolmogorov_smirnov_cdf)

# ------------------------------------------------------------------------
#
# normal_critical_values
#

add_executable(normal_critical_values normal_critical_values.cpp)

target_link_libraries(normal_critical_values libgama)

add_test(NAME normal_critical_values COMMAND normal_critical_values)

# ------------------------------------------------------------------------
#
# normal_distribution
#

add_executable(normal_distribution normal_distribution.cpp)

target_link_libraries(normal_distribution libgama)

add_test(NAME normal_distribution COMMAND normal_distribution)

# ------------------------------------------------------------------------
#
# student_critical_values
#

add_executable(student_critical_values student_critical_values.cpp)

target_link_libraries(student_critical_values libgama)

add_test(NAME student_critical_values COMMAND student_critical_values)