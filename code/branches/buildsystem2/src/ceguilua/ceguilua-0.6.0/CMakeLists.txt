# Copy all files from the package folder to the binary directory

SET(_package_files
  CEGUI.pkg
  Logger.pkg
  ScriptWindowHelper.pkg
  WindowManager.pkg
  elements/GroupBox.pkg
  elements/MultiLineEditbox.pkg
  elements/Tree.pkg
  elements/TreeItem.pkg
)

FOREACH(_file ${_package_files})
  CONFIGURE_FILE(package/${_file} ${CEGUILUA_BINARY_DIR}/${_file} COPYONLY)
ENDFOREACH(_file)
