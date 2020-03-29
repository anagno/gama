set(TEST_BASE_DIR ${PROJECT_SOURCE_DIR}/tests/gama-local)
set(INPUT_DIR ${TEST_BASE_DIR}/input)
set(INPUT_FILES
    gama-local
    zoltan-test_2d_dms
    zoltan-test_2d_gon
    zoltan-test_3d_dms
    zoltan-test_3d_gon
    tst-tetrahedron-1
    tst-tetrahedron-2
    tst-tetrahedron-3-deg
    tst-tetrahedron-3-gon
    fixed-azimuth
    azimuth-angle
    azimuth-azimuth
    azimuth-distance
    jezerka-ang
    jezerka-dir
    krasovsky-1926
    triangle-1
    triangle-2
    stroner-levelling-a
    stroner-levelling-b
    extern-azimuth-distance
    extern-tst-tetrahedron-3-gon
    extern-seq-dsuloha-d
    bug-2019-08-06-integer-coordinates
    bug-2019-08-13-traverse-knin_male
    bug-2019-08-20-knin_test
  )

set(RESULT_DIR ${CMAKE_BINARY_DIR}/tests/gama-local/results/${PROJECT_VERSION})
file(MAKE_DIRECTORY ${RESULT_DIR})


# ------------------------------------------------------------------------
#
# check version
#
add_executable(check_version scripts/check_version.cpp
  )

target_link_libraries(check_version libgama)

add_test(NAME check_version COMMAND check_version
  ${PROJECT_SOURCE_DIR}/CMakeLists.txt)

# -------------------------------------------------------------------------
#
# check_adjustment
#

file(MAKE_DIRECTORY ${RESULT_DIR}/gama-local-adjustment)

foreach(test ${INPUT_FILES})
  foreach(algo svd gso cholesky envelope)
    add_test(NAME gama_local_adjustement_${test}_${algo}
      COMMAND gama-local ${INPUT_DIR}/${test}.gkf --algorithm ${algo}
        --text   ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.txt
        --html   ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.html
        --xml    ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.xml
        --octave ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.m
        --svg    ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.svg
      )
  endforeach(algo)
endforeach(test)

# -------------------------------------------------------------------------
#
# check_adjustment 360
# or testing text and html output in 360 degrees we use gso algorithm only
#

foreach(test ${INPUT_FILES})
  foreach(algo gso) # svd  cholesky envelope)
    add_test(NAME gama_local_adjustement_360_${test}_${algo}
      COMMAND gama-local ${INPUT_DIR}/${test}.gkf --algorithm ${algo}
        --text   ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}-360.txt
        --html   ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}-360.html
      )
  endforeach(algo)
endforeach(test)



# ------------------------------------------------------------------------
#
# check_algorithms
#
add_executable(check_algorithms scripts/check_xyz.h scripts/check_xyz.cpp
  scripts/check_algorithms.cpp)

target_link_libraries(check_algorithms libgama)

foreach(test ${INPUT_FILES})
  add_test(NAME check_algorithms_${test}
    COMMAND check_algorithms ${PROJECT_VERSION} ${test}
    ${INPUT_DIR}/${test}.gkf )
endforeach(test)



# ------------------------------------------------------------------------
#
# check_xml_xml
#
add_executable(check_xml_xml scripts/check_xml_xml.cpp
  scripts/compare_xml_adjustment.h scripts/compare_xml_adjustment.cpp
  )

target_link_libraries(check_xml_xml libgama)

set(RES ${RESULT_DIR}/gama-local-adjustment)

foreach(z ${INPUT_FILES})
  foreach(algorithms gso:svd gso:cholesky gso:envelope
                     svd:cholesky svd:envelope cholesky:envelope)
    string(REPLACE ":" ";" test_list ${algorithms})
    list(GET test_list 0 a)
    list(GET test_list 1 b)
    add_test(NAME check_xml_xml_${z}_${a}_${b} COMMAND
      check_xml_xml "${a} ${b} ${z}" ${RES}/${z}-${a}.xml ${RES}/${z}-${b}.xml
    )

    set_tests_properties(check_xml_xml_${z}_${a}_${b}
                         PROPERTIES DEPENDS
                                    gama_local_adjustement_${z}-${a})

    set_tests_properties(check_xml_xml_${z}_${a}_${b}
                         PROPERTIES DEPENDS
                                    gama_local_adjustement_${z}-${b})
  endforeach(algorithms)
endforeach(z)

# ------------------------------------------------------------------------
#
# check html
#
add_executable(check_html scripts/check_html.cpp
                          scripts/compare_xml_adjustment.h
                          scripts/compare_xml_adjustment.cpp)

target_link_libraries(check_html libgama)

foreach(test ${INPUT_FILES})
  foreach(algo gso) # svd  cholesky envelope)
    add_test(NAME check_html_${test}_${algo}
             COMMAND check_html
                     ${test}
                     ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.html
                     ${INPUT_DIR}/${test}.xml)

    set_tests_properties(check_html_${test}_${algo}
                         PROPERTIES DEPENDS
                                    gama_local_adjustement_${test}_${algo})
  endforeach(algo)
endforeach(test)

foreach(test ${INPUT_FILES})
  foreach(algo gso) # svd  cholesky envelope)
    add_test(NAME check_html_360_${test}_${algo}
             COMMAND check_html
                     ${test}-360
                     ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}-360.html
                     ${INPUT_DIR}/${test}.xml)

    set_tests_properties(check_html_360_${test}_${algo}
                         PROPERTIES DEPENDS
                                    gama_local_adjustement_360_${test}_${algo})
  endforeach(algo)
endforeach(test)


# -------------------------------------------------------------------------
#
# check_equivalents
#
add_executable(check_equivalents scripts/check_xyz.h
  scripts/check_xyz.cpp scripts/check_equivalents.cpp
  )

target_link_libraries(check_equivalents libgama)

set(EQUIVALENTS_TESTS
    # CMake is weird with 2D arrays, so we apply this hack
    # format: <test_name>;<input_a>;<input_b>
    zoltan_2d:zoltan-test_2d_dms:zoltan-test_2d_gon
    zoltan_3d:zoltan-test_3d_dms:zoltan-test_3d_gon
    scale:scale-cov-gon:scale-cov-dms
    fixed:fixed-constrained:fixed-azimuth
    gama-local:gama-local:gama-local-deprecated
    stroner:stroner-levelling-a:stroner-levelling-b
    tetrahedron:tst-tetrahedron-3-gon:extern-tst-tetrahedron-3-gon
    azimuth:azimuth-distance:extern-azimuth-distance
)

foreach(test ${EQUIVALENTS_TESTS})
    string(REPLACE ":" ";" TEST_LIST ${test})
    list(GET TEST_LIST 0 testname)
    list(GET TEST_LIST 1 input_a)
    list(GET TEST_LIST 2 input_b)
    add_test(
        NAME check_equivalents_${testname}
        COMMAND check_equivalents ${input_a}
        ${INPUT_DIR}/${input_a}.gkf ${input_b} ${INPUT_DIR}/${input_b}.gkf
    )
endforeach(test)

# -------------------------------------------------------------------------
#
# check_xml_results
#
add_executable(check_xml_results  scripts/check_xml_results.cpp
  scripts/check_xyz.h scripts/check_xyz.cpp)

target_link_libraries(check_xml_results libgama)  

foreach(test ${INPUT_FILES})
  add_test(
    NAME check_xml_results_${test}
    COMMAND check_xml_results ${test}
      ${INPUT_DIR}/${test}.gkf ${INPUT_DIR}/${test}.xml
  )
endforeach(test)

# ------------------------------------------------------------------------
#
# check externs
#
add_executable(check_externs scripts/check_externs.cpp ../acord2/acord.cpp ../acord2/acord.h)

target_link_libraries(check_externs libgama)  

add_test(NAME check_externs COMMAND check_externs
  ${INPUT_DIR} ${RESULT_DIR}/gama-local-adjustment ${INPUT_FILES})

foreach(test ${INPUT_FILES})
  foreach(algo gso) # svd  cholesky envelope)
    add_test(NAME check_externs_${test}_${algo}
             COMMAND check_externs
                     ${INPUT_DIR}
                     ${RESULT_DIR}/gama-local-adjustment
                     ${test})

    set_tests_properties(check_externs_${test}_${algo}
                         PROPERTIES DEPENDS
                                    gama_local_adjustement_${test}_${algo})
  endforeach(algo)
endforeach(test)

if(LibXml2_FOUND)

  set(GAMA_XML "${PROJECT_SOURCE_DIR}/Parsing/Schemas")

  foreach(test ${INPUT_FILES})
    add_test(NAME xmllint_gama_local_xsd_${test}
      COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
        --schema ${GAMA_XML}/gama-local.xsd
        ${INPUT_DIR}/${test}.gkf
        --noout)
  endforeach(test)

  file(MAKE_DIRECTORY ${RESULT_DIR}/xmllint-gama-local-adjustment-xsd)

  foreach(test ${INPUT_FILES})
    add_test(NAME xmllint-gama-local-adjustment-xsd_input_${test}
      COMMAND gama-local ${INPUT_DIR}/${test}.gkf
        --xml ${RESULT_DIR}/xmllint-gama-local-adjustment-xsd/${test}.xml)
  endforeach(test)

  foreach(test ${INPUT_FILES})
    add_test(NAME xmllint-gama-local-adjustment-xsd_output_${test}
      COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
        --schema ${GAMA_XML}/gama-local-adjustment.xsd
        --noout
        ${RESULT_DIR}/xmllint-gama-local-adjustment-xsd/${test}.xml
        )

    set_tests_properties(xmllint-gama-local-adjustment-xsd_output_${test}
                         PROPERTIES DEPENDS
                                    xmllint-gama-local-adjustment-xsd_input_${test})
  endforeach(test)

endif()


if(OCTAVE_FOUND)

  foreach(test ${INPUT_FILES})
    foreach(algo svd gso cholesky envelope)
      add_test(NAME gama_local_octave_${test}_${algo}
               COMMAND ${OCTAVE_EXECUTABLE}
                       ${RESULT_DIR}/gama-local-adjustment/${test}-${algo}.m)

      set_tests_properties(gama_local_octave_${test}_${algo}
                           PROPERTIES DEPENDS gama_local_adjustement_${test}_${algo})
    endforeach(algo)
  endforeach(test)

endif()


# -------------------------------------------------------------------------
#
# gama-local-parameters
#

file(MAKE_DIRECTORY ${RESULT_DIR}/gama-local-parameters)

add_test(NAME gama_local_par
  COMMAND gama-local ${INPUT_DIR}/gama-local-par.gkf
    --xml ${RESULT_DIR}/gama-local-parameters/par.xml
)

add_test(NAME gama_local_nop
  COMMAND gama-local ${INPUT_DIR}/gama-local-nop.gkf
    --xml ${RESULT_DIR}/gama-local-parameters/nop.xml
    --algorithm gso
    --cov-band 1
    --latitude 50.2
    --ellipsoid wgs84
)

add_test(NAME gama_local_par_nop_compare 
  COMMAND ${CMAKE_COMMAND} -E compare_files ${RESULT_DIR}/gama-local-parameters/nop.xml 
  ${RESULT_DIR}/gama-local-parameters/par.xml
)

set_tests_properties(gama_local_par_nop_compare 
  PROPERTIES DEPENDS gama_local_nop)

set_tests_properties(gama_local_par_nop_compare 
  PROPERTIES DEPENDS gama_local_nop)

add_test(NAME gama_local_par_xml2txt 
  COMMAND gama-local-xml2txt ${RESULT_DIR}/gama-local-parameters/par.xml ${RESULT_DIR}/gama-local-parameters/par.txt
)

set_tests_properties(gama_local_par_xml2txt 
  PROPERTIES DEPENDS gama_local_par)

add_test(NAME gama_local_nop_xml2txt 
  COMMAND gama-local-xml2txt ${RESULT_DIR}/gama-local-parameters/nop.xml ${RESULT_DIR}/gama-local-parameters/nop.txt
)

set_tests_properties(gama_local_nop_xml2txt 
  PROPERTIES DEPENDS gama_local_nop)


if(LibXml2_FOUND)

  set(GAMA_XML "${PROJECT_SOURCE_DIR}/Parsing/Schemas")

  add_test(NAME xmllint_gama_local_par
    COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
      --schema ${GAMA_XML}/gama-local.xsd
      ${INPUT_DIR}/gama-local-par.gkf
      --noout)

  add_test(NAME xmllint_gama_local_nop
    COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
      --schema ${GAMA_XML}/gama-local.xsd
      ${INPUT_DIR}/gama-local-nop.gkf
      --noout)

  add_test(NAME xmllint_gama_local_par_xml2txt 
    COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
      --schema ${GAMA_XML}/gama-local-adjustment.xsd
      ${RESULT_DIR}/gama-local-parameters/par.xml
      --noout)

  set_tests_properties(xmllint_gama_local_par_xml2txt 
      PROPERTIES DEPENDS gama_local_par)

  add_test(NAME xmllint_gama_local_nop_xml2txt 
    COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
      --schema ${GAMA_XML}/gama-local-adjustment.xsd
      ${RESULT_DIR}/gama-local-parameters/nop.xml
      --noout)

  set_tests_properties(xmllint_gama_local_nop_xml2txt 
      PROPERTIES DEPENDS gama_local_nop)

endif()

# -------------------------------------------------------------------------
#
# gama-local-updated-xml
#

file(MAKE_DIRECTORY ${RESULT_DIR}/gama-local-updated-xml)

set(INPUT_FILES_LOCAL_UPDATED
  gama-local.gkf
  azimuth-angle.gkf
  prostorovka.gkf
  skorepa-dusek.gkf
  mikhail-7.4.gkf
  minimal.gkf
  seq-dsuloha-d.gkf
  tst-tetrahedron-2.gkf
  tst-tetrahedron-3-gon.gkf
  tst-tetrahedron-3-deg.gkf)

foreach(test ${INPUT_FILES_LOCAL_UPDATED})

  add_test(NAME gama_local_updated_xml_${test}
    COMMAND gama-local ${INPUT_DIR}/${test}
      --xml ${RESULT_DIR}/gama-local-updated-xml/${test}-1.xml
      --updated-xml ${RESULT_DIR}/gama-local-updated-xml/${test}-1.gkf
    )

  add_test(NAME gama_local_updated_xml_2_${test}
    COMMAND gama-local ${RESULT_DIR}/gama-local-updated-xml/${test}-1.gkf
      --xml ${RESULT_DIR}/gama-local-updated-xml/${test}-2.xml
    )

  set_tests_properties(gama_local_updated_xml_2_${test} 
      PROPERTIES DEPENDS gama_local_updated_xml_${test})

  add_test(NAME check_xml_xml_update_xml_${test}
    COMMAND check_xml_xml "udpated xml ${test}" 
      ${RESULT_DIR}/gama-local-updated-xml/${test}-1.xml
      ${RESULT_DIR}/gama-local-updated-xml/${test}-2.xml
    )

  set_tests_properties(check_xml_xml_update_xml_${test}
    PROPERTIES DEPENDS gama_local_updated_xml_${test})

  set_tests_properties(check_xml_xml_update_xml_${test}
    PROPERTIES DEPENDS gama_local_updated_xml_2_${test})


  if(LibXml2_FOUND)

    add_test(NAME xmllint_gama_local_update_xml_${test}
      COMMAND ${LIBXML2_XMLLINT_EXECUTABLE}
        --schema ${GAMA_XML}/gama-local.xsd
        ${RESULT_DIR}/gama-local-updated-xml/${test}-1.gkf
        --noout)

    set_tests_properties(xmllint_gama_local_update_xml_${test}
      PROPERTIES DEPENDS gama_local_updated_xml_${test})

  endif()

endforeach(test)


# -------------------------------------------------------------------------
#
# gama-local-sqlite-reader
#

if(SQLITE3_FOUND)

  add_executable(sqlite_init_db  scripts/sqlite_init_db.cpp)

  target_link_libraries(sqlite_init_db libgama sqlite3::sqlite3)

  add_executable(check_xml_coordinates  scripts/check_xml_coordinates.cpp)

  target_link_libraries(check_xml_coordinates libgama)


  file(MAKE_DIRECTORY ${RESULT_DIR}/gama-local-sqlite-reader)
  set(GAMA_XML "${PROJECT_SOURCE_DIR}/Parsing/Schemas")

  add_test(NAME gama_local_xml2sql_fixed-azimuth
    COMMAND gama-local-xml2sql
      fixed-azimuth
      ${INPUT_DIR}/fixed-azimuth.gkf
      ${RESULT_DIR}/gama-local-sqlite-reader/fixed-azimuth.sql
  )

  add_test(NAME sqlite_init_db_fixed-azimuth
    COMMAND sqlite_init_db
      ${RESULT_DIR}/gama-local-sqlite-reader/demo.db
      ${GAMA_XML}/gama-local-schema.sql
      ${RESULT_DIR}/gama-local-sqlite-reader/fixed-azimuth.sql
  )

  set_tests_properties(sqlite_init_db_fixed-azimuth
    PROPERTIES DEPENDS gama_local_xml2sql_fixed-azimuth
               RUN_SERIAL TRUE)

  add_test(NAME gama_local_adjustement_sql_fixed-azimuth
    COMMAND gama-local 
    --sqlitedb ${RESULT_DIR}/gama-local-sqlite-reader/demo.db
    --readonly-configuration fixed-azimuth
    --xml ${RESULT_DIR}/gama-local-sqlite-reader/fixed-azimuth.xml
    )

  add_test(NAME gama_local_adjustement_sql_fixed-constrained
    COMMAND gama-local ${INPUT_DIR}/fixed-constrained.gkf
    --xml ${RESULT_DIR}/gama-local-sqlite-reader/fixed-constrained.xml
    )

  set_tests_properties(gama_local_adjustement_sql_fixed-azimuth
    PROPERTIES DEPENDS sqlite_init_db_fixed-azimuth
               RUN_SERIAL TRUE)

  add_test(NAME check_xml_coordinates_fixed-azimuth
    COMMAND check_xml_coordinates
    ${RESULT_DIR}/gama-local-sqlite-reader/fixed-azimuth.xml
    ${RESULT_DIR}/gama-local-sqlite-reader/fixed-constrained.xml
  )

  set_tests_properties(check_xml_coordinates_fixed-azimuth
    PROPERTIES DEPENDS gama_local_adjustement_sql_fixed-azimuth
               RUN_SERIAL TRUE)

  set_tests_properties(check_xml_coordinates_fixed-azimuth
    PROPERTIES DEPENDS gama_local_adjustement_sql_fixed-constrained
               RUN_SERIAL TRUE)

  set(INPUT_FILES_SQL
    tst-tetrahedron-2
    tst-tetrahedron-3-gon
    gama-local
    zoltan-test_2d_dms
    zoltan-test_3d_gon
    )

  foreach(test ${INPUT_FILES_SQL})

    add_test(NAME gama_local_xml2sql_${test}
      COMMAND gama-local-xml2sql
        ${test}
        ${INPUT_DIR}/${test}.gkf
        ${RESULT_DIR}/gama-local-sqlite-reader/${test}.sql
    )

    add_test(NAME sqlite_init_db_${test}
      COMMAND sqlite_init_db
        ${RESULT_DIR}/gama-local-sqlite-reader/demo.db
        ${RESULT_DIR}/gama-local-sqlite-reader/${test}.sql
    )

    set_tests_properties(sqlite_init_db_${test}
      PROPERTIES DEPENDS gama_local_xml2sql_${test}
                 RUN_SERIAL TRUE)

    set_tests_properties(sqlite_init_db_${test}
      PROPERTIES DEPENDS sqlite_init_db_fixed-azimuth)

    set_tests_properties(sqlite_init_db_${test}
      PROPERTIES DEPENDS gama_local_adjustement_sql_fixed-azimuth)

    add_test(NAME gama_local_adjustement_sql_${test}
      COMMAND gama-local 
      --sqlitedb ${RESULT_DIR}/gama-local-sqlite-reader/${test}.db
      --readonly-configuration ${test}
      --xml ${RESULT_DIR}/gama-local-sqlite-reader/${test}.xml
      )

    set_tests_properties(gama_local_adjustement_sql_${test}
      PROPERTIES DEPENDS sqlite_init_db_${test}
                 RUN_SERIAL TRUE)

    add_test(NAME check_xml_coordinates_${test}
      COMMAND check_xml_coordinates
      ${RESULT_DIR}/gama-local-sqlite-reader/${test}.xml
      ${INPUT_DIR}/${test}.xml
    )

    set_tests_properties(check_xml_coordinates_${test}
      PROPERTIES DEPENDS gama_local_adjustement_sql_${test}
                 RUN_SERIAL TRUE)

  endforeach(test)

endif()
