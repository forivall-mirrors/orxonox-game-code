# Copy all files from the package folder to the binary directory

SET(_package_files
  EventSet.pkg
  HelperFunctions.pkg
  InputEvent.pkg
  Window.pkg
  WindowManager.pkg
)

FOREACH(_file ${_package_files})
  CONFIGURE_FILE(package/${_file} ${CEGUILUA_BINARY_DIR}/${_file} COPYONLY)
ENDFOREACH(_file)
