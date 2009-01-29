# Copy all files from the package folder to the binary directory

# Only do this once since GLOB_RECURSE is quite slow
# No, don't. When switch back from 0.6 to 0.5 and keeping the binary dir
# This will cause problems.
#IF(EXISTS ${CEGUILUA_BINARY_DIR}/exceptions.lua)
#  RETURN()
#ENDIF(EXISTS ${CEGUILUA_BINARY_DIR}/exceptions.lua)

FILE(GLOB_RECURSE _package_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/package "*.pkg")
FOREACH(_file ${_package_files} "exceptions.lua")
  CONFIGURE_FILE(package/${_file} ${CEGUILUA_BINARY_DIR}/${_file} COPYONLY)
ENDFOREACH(_file)
