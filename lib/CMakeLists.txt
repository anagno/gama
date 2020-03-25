# add_definitions(-DDEBUG_ACORD2) add_definitions(-DA2G_DEBUG)

# Library
set(SRC_GAMA
   gnu_gama/exception.h
   gnu_gama/version.h
   gnu_gama/xsd.h

   gnu_gama/xml/baseparser.cpp
   gnu_gama/xml/baseparser.h
   gnu_gama/xml/htmlparser.cpp
   gnu_gama/xml/htmlparser.h
   gnu_gama/xml/localnetworkoctave.cpp
   gnu_gama/xml/localnetworkoctave.h
   gnu_gama/xml/localnetworksql.cpp
   gnu_gama/xml/localnetworksql.h
   gnu_gama/xml/localnetworkxml.cpp
   gnu_gama/xml/localnetworkxml.h
   gnu_gama/xml/localnetwork_adjustment_results.cpp
   gnu_gama/xml/localnetwork_adjustment_results.h
   gnu_gama/xml/localnetwork_adjustment_results_data.cpp
   gnu_gama/xml/localnetwork_adjustment_results_data.h
   gnu_gama/xml/gkfparser.cpp
   gnu_gama/xml/gkfparser.h

   gnu_gama/local/angobs.h
   gnu_gama/local/bearing.cpp
   gnu_gama/local/bearing.h
   gnu_gama/local/html.cpp
   gnu_gama/local/html.h
   gnu_gama/local/cluster.h
   gnu_gama/local/exception.h
   gnu_gama/local/itstream.h
   gnu_gama/local/svg.cpp
   gnu_gama/local/svg.h
   gnu_gama/local/acord/approx_heights.cpp
   gnu_gama/local/acord/approx_heights.h
   gnu_gama/local/acord/approx_vectors.cpp
   gnu_gama/local/acord/approx_vectors.h
   gnu_gama/local/acord/acord2.cpp
   gnu_gama/local/acord/acord2.h
   gnu_gama/local/acord/acordalgorithm.cpp
   gnu_gama/local/acord/acordalgorithm.h
   gnu_gama/local/acord/acordazimuth.cpp
   gnu_gama/local/acord/acordazimuth.h
   gnu_gama/local/acord/acordhdiff.cpp
   gnu_gama/local/acord/acordhdiff.h
   gnu_gama/local/acord/acordintersection.cpp
   gnu_gama/local/acord/acordintersection.h
   gnu_gama/local/acord/acordpolar.cpp
   gnu_gama/local/acord/acordpolar.h
   gnu_gama/local/acord/acordstatistics.h
   gnu_gama/local/acord/acordstatistics.cpp
   gnu_gama/local/acord/acordtraverse.cpp
   gnu_gama/local/acord/acordtraverse.h
   gnu_gama/local/acord/acordvector.cpp
   gnu_gama/local/acord/acordvector.h
   gnu_gama/local/acord/acordweakchecks.cpp
   gnu_gama/local/acord/acordweakchecks.h
   gnu_gama/local/acord/acordzderived.cpp
   gnu_gama/local/acord/acordzderived.h
   gnu_gama/local/acord/reduce_observations.cpp
   gnu_gama/local/acord/reduce_observations.h
   gnu_gama/local/acord/reduce_to_ellipsoid.cpp
   gnu_gama/local/acord/reduce_to_ellipsoid.h
   gnu_gama/local/format.cpp
   gnu_gama/local/format.h
   gnu_gama/local/gamadata.cpp
   gnu_gama/local/gamadata.h
   gnu_gama/local/lcoords.cpp
   gnu_gama/local/lcoords.h
   gnu_gama/local/local_linearization.cpp
   gnu_gama/local/local_linearization.h
   gnu_gama/local/localnetwork2sql.cpp
   gnu_gama/local/localnetwork2sql.h
   gnu_gama/local/median/g2d_cogo.cpp
   gnu_gama/local/median/g2d_cogo.h
   gnu_gama/local/median/g2d_coordinates.cpp
   gnu_gama/local/median/g2d_coordinates.h
   gnu_gama/local/median/g2d_exception.h
   gnu_gama/local/median/g2d_helper.cpp
   gnu_gama/local/median/g2d_helper.h
   gnu_gama/local/median/g2d_point.cpp
   gnu_gama/local/median/g2d_point.h
   gnu_gama/local/network.cpp
   gnu_gama/local/network.h
   gnu_gama/local/orientation.cpp
   gnu_gama/local/orientation.h
   gnu_gama/local/results/text/adjusted_observations.h
   gnu_gama/local/results/text/adjusted_unknowns.h
   gnu_gama/local/results/text/approximate_coordinates.h
   gnu_gama/local/results/text/error_ellipses.h
   gnu_gama/local/results/text/fixed_points.h
   gnu_gama/local/results/text/general_parameters.h
   gnu_gama/local/results/text/network_description.h
   gnu_gama/local/results/text/outlying_abs_terms.h
   gnu_gama/local/results/text/reduced_observations.h
   gnu_gama/local/results/text/reduced_observations_to_ellipsoid.h
   gnu_gama/local/results/text/residuals_observations.h
   gnu_gama/local/results/text/underline.cpp
   gnu_gama/local/results/text/underline.h
   gnu_gama/local/test_linearization_visitor.cpp
   gnu_gama/local/test_linearization_visitor.h
   gnu_gama/local/local_revision.cpp
   gnu_gama/local/local_revision.h
   gnu_gama/local/matvec.h
   gnu_gama/local/medianf.h
   gnu_gama/local/observation.cpp
   gnu_gama/local/observation.h
   gnu_gama/local/pointid.cpp
   gnu_gama/local/pointid.h
   gnu_gama/local/readsabw.h
   gnu_gama/local/skipcomm.cpp
   gnu_gama/local/skipcomm.h
   gnu_gama/local/sqlitereader.h
   gnu_gama/local/sqlitereader.cpp
   gnu_gama/local/writevisitor.h
   gnu_gama/local/xmlerror.h
   gnu_gama/local/xmlerror.cpp

   gnu_gama/g3/g3_adjres.cpp
   gnu_gama/g3/g3_adjres.h
   gnu_gama/g3/g3_cluster.cpp
   gnu_gama/g3/g3_cluster.h
   gnu_gama/g3/g3_model.cpp
   gnu_gama/g3/g3_model.h
   gnu_gama/g3/g3_model_init.cpp
   gnu_gama/g3/g3_model_linearization.cpp
   gnu_gama/g3/g3_model_revision.cpp
   gnu_gama/g3/g3_model_write_xml_adjustment_results.cpp
   gnu_gama/g3/g3_observation.h
   gnu_gama/g3/g3_parameter.h
   gnu_gama/g3/g3_point.cpp
   gnu_gama/g3/g3_point.h
   gnu_gama/g3/g3_write_observation_xml.cpp
   gnu_gama/g3/g3_write_observation_xml.h

   gnu_gama/xml/dataobject.cpp
   gnu_gama/xml/dataobject.h
   gnu_gama/xml/dataparser_adj.cpp
   gnu_gama/xml/dataparser.cpp
   gnu_gama/xml/dataparser_g3.cpp
   gnu_gama/xml/dataparser_g3adj.cpp
   gnu_gama/xml/dataparser.h
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama)

add_custom_command(OUTPUT gnu_gama/ellipsoids.cpp
                          gnu_gama/ellipsoids.h
                          gnu_gama/ellipsoids.html
                          gnu_gama/ellipsoids.texi
                   DEPENDS BuildTools
                   COMMAND ${BuildTools_ellipsoids_xml}
                           ${Parsing_SOURCE_DIR}/Schemas/ellipsoids.xml
                           ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama)

# file(GLOB_RECURSE) commands is being misused here
# Normally his should be avoided because if files are added or removed, 
# CMake is not automatically re-run, so the build is unaware of the change.
# https://cmake.org/cmake/help/v3.10/command/file.html
file(GLOB_RECURSE language_files
  ${Parsing_SOURCE_DIR}/Schemas/lang/*/*.lang
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local)

add_custom_command(OUTPUT gnu_gama/local/language.h
                          gnu_gama/local/language.cpp
                   DEPENDS BuildTools
                   COMMAND ${BuildTools_slovnikar} ${language_files}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local)

# https://crascit.com/2017/04/18/generated-sources-in-cmake-builds/
# https://cmake.org/cmake/help/v3.10/variable/CMAKE_VERSION.html
configure_file(gnu_gama/version.cpp.in gnu_gama/version.cpp @ONLY)

add_library(libgama ${SRC_GAMA}
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/ellipsoids.cpp
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/ellipsoids.h
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local/language.h
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/local/language.cpp
                    ${CMAKE_CURRENT_BINARY_DIR}/gnu_gama/version.cpp
                    )

target_include_directories(libgama
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                  ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(libgama 
  PUBLIC 
    GaMa::Service_Parsing 
    GaMa::Business_Parsing 
    GaMa::Service_Math 
    GaMa::Business_MathCore 
    GaMa::Business_MathAdjustment
    GaMa::Service_Utilities)

include(GenerateExportHeader)
generate_export_header(libgama
                       EXPORT_FILE_NAME
                       GaMaDLL.h
                       EXPORT_MACRO_NAME
                       GaMaAPI)

if(SQLITE3_FOUND)
  if(USE_SQLITE3)
    target_link_libraries(libgama PRIVATE sqlite3::sqlite3)
  endif()
endif()
