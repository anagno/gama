set(INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/input)

include(CMake_TestFiles.cmake)

set(RESULT_DIR ${CMAKE_CURRENT_BINARY_DIR}/results/${PROJECT_VERSION})
file(MAKE_DIRECTORY ${RESULT_DIR})

# -------------------------------------------------------------------------
#
# Generate the results
#

file(MAKE_DIRECTORY ${RESULT_DIR}/gama-g3-adjustment)
file(MAKE_DIRECTORY ${RESULT_DIR}/gama-g3-adjustment/sjtsk05)

foreach(test ${INPUT_FILES})
  foreach(algo
          envelope
          gso
          cholesky
          svd)
    add_test(NAME gama_g3_adjustement_${test}_${algo}
             COMMAND gama-g3
                     --algorithm
                     ${algo}
                     ${INPUT_DIR}/${test}.xml
                     ${RESULT_DIR}/gama-g3-adjustment/${test}-${algo}.adj.xml)
  endforeach(algo)
endforeach(test)

foreach(test ${SJTSK05_FILES})
  add_test(
    NAME gama_g3_adjustement_${test}
    COMMAND gama-g3
            --algorithm
            envelope
            ${INPUT_DIR}/sjtsk05/${test}.xml
            ${RESULT_DIR}/gama-g3-adjustment/sjtsk05/${test}-envelope.adj.xml)
endforeach(test)

# ------------------------------------------------------------------------
#
# check_adjustment
#
add_executable(check_g3_adjustment scripts/check_adjustment.cpp)

target_link_libraries(check_g3_adjustment libgama)

foreach(test ${INPUT_FILES})
  foreach(algo
          envelope
          gso
          cholesky
          svd)
    add_test(NAME check_g3_adjustment_${test}_${algo}
             COMMAND check_g3_adjustment ${INPUT_DIR}/${test}-adj.xml
                     ${RESULT_DIR}/gama-g3-adjustment/${test}-${algo}.adj.xml)

    set_tests_properties(check_g3_adjustment_${test}_${algo}
                         PROPERTIES DEPENDS gama_g3_adjustement_${test}_${algo})
  endforeach(algo)
endforeach(test)

foreach(test ${SJTSK05_FILES})
  add_test(
    NAME check_g3_adjustment_${test}
    COMMAND check_g3_adjustment ${INPUT_DIR}/sjtsk05/${test}.adj.xml
            ${RESULT_DIR}/gama-g3-adjustment/sjtsk05/${test}-envelope.adj.xml)

  set_tests_properties(check_g3_adjustment_${test}
                       PROPERTIES DEPENDS gama_g3_adjustement_${test})
endforeach(test)

if(LIBXML2_FOUND)

  set(GAMA_XML "${PROJECT_SOURCE_DIR}/Parsing/Schemas")

  foreach(test ${INPUT_FILES})
    add_test(NAME xmllint_g3_adjustement_xsd_input_${test}
             COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
                     --schema
                     ${GAMA_XML}/gnu-gama-data.xsd
                     ${INPUT_DIR}/${test}.xml
                     --noout)
  endforeach(test)

  foreach(test ${INPUT_FILES})
    add_test(NAME xmllint_g3_adjustement_xsd_input_adj_${test}
             COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
                     --schema
                     ${GAMA_XML}/gnu-gama-data.xsd
                     ${INPUT_DIR}/${test}-adj.xml
                     --noout)
  endforeach(test)

  foreach(test ${INPUT_FILES})
    foreach(algo
            envelope
            gso
            cholesky
            svd)
      add_test(NAME xmllint_g3_adjustement_xsd_output_${test}_${algo}
               COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
                       --schema
                       ${GAMA_XML}/gnu-gama-data.xsd
                       ${RESULT_DIR}/gama-g3-adjustment/${test}-${algo}.adj.xml
                       --noout)

      set_tests_properties(xmllint_g3_adjustement_xsd_output_${test}_${algo}
                           PROPERTIES DEPENDS
                                      gama_g3_adjustement_${test}_${algo})
    endforeach(algo)
  endforeach(test)

  foreach(test ${SJTSK05_FILES})

    add_test(NAME xmllint_g3_adjustement_xsd_output_${test}_${algo}
             COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
                     --schema
                     ${GAMA_XML}/gnu-gama-data.xsd
                     ${RESULT_DIR}/gama-g3-adjustment/sjtsk05/${test}-envelope.adj.xml
                     --noout)

    set_tests_properties(xmllint_g3_adjustement_xsd_output_${test}_${algo}
                         PROPERTIES DEPENDS gama_g3_adjustement_${test})
  endforeach(test)

endif()
