# Eventually this schemas files should go under their respective projects
# I am creating this interface library so I can find these files from the UT

set(schemas_files
  "ellipsoids.xml"
  "gama-local-adjustment.xml"
  "gama-local-adjustment.xml"
  "gama-local-adjustment.xsd"
  "gama-local-schema.sql"
  "gama-local.xml"
  "gama-local.xsd"
  "gnu-gama-data.xsd"
)

install(
  FILES ${schemas_files}
  DESTINATION ${CMAKE_INSTALL_DATADIR}/Schemas)
