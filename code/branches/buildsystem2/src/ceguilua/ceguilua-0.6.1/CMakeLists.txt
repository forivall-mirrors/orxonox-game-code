# Copy all files from the package folder to the binary directory

SET(_package_files
  Window.pkg
)

FOREACH(_file ${_package_files})
  CONFIGURE_FILE(package/${_file} ${CEGUILUA_BINARY_DIR}/${_file} COPYONLY)
ENDFOREACH(_file)
