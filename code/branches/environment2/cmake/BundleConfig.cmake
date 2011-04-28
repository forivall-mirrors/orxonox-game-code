#
#             ORXONOX - the hottest 3D action shooter ever to exist
#                             > www.orxonox.net <
#
#        This program is free software; you can redistribute it and/or
#         modify it under the terms of the GNU General Public License
#        as published by the Free Software Foundation; either version 2
#            of the License, or (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#        but WITHOUT ANY WARRANTY; without even the implied warranty of
#        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#                 GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#      with this program; if not, write to the Free Software Foundation,
#     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
#
#  Author:
#    Kevin Young
#  Description:
#    Sets all necessary variables for CPack package generation
#

# Common properties across all generators

#SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
SET(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_BINARY_DIR};${PROJECT_NAME};ALL;/)
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME}")
SET(CPACK_PACKAGE_EXECUTABLES ${ORXONOX_EXECUTABLE_NAME};${PROJECT_NAME})
SET(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME} ${ORXONOX_VERSION_NAME}")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PROJECT_NAME} ${ORXONOX_VERSION}")
# Defaults to PROJECT_NAME anyway
#SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_VENDOR "www.orxonox.net")
SET(CPACK_PACKAGE_VERSION_MAJOR ${ORXONOX_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${ORXONOX_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${ORXONOX_VERSION_PATCH})
#SET(CPACK_PROJECT_CONFIG_FILE ${CMAKE_BINARY_DIR}/CPackOptions.cmake)
SET(CPACK_SOURCE_GENERATOR TGZ;ZIP)
# Unsure what pattern to set here
#SET(CPACK_SOURCE_IGNORE_FILES /CVS/;/\\.svn/;\\.swp$;\\.#;/#;.*~;cscope.*)
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${ORXONOX_VERSION}")
#SET(CPACK_SOURCE_STRIP_FILES FALSE)
# Defaults to CMAKE_SYSTEM_NAME anyway
#SET(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")

# Advanced settings
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENCE")
  
# Apple specific settings
IF(APPLE)
  SET(CPACK_GENERATOR "Bundle")

  SET(CPACK_BUNDLE_NAME "${CPACK_PACKAGE_FILE_NAME}")
  SET(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/Icon.icns")
  SET(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/${DEFAULT_DATA_PATH}/mac/Orxonox-Info.plist")
  SET(CPACK_BUNDLE_STARTUP_COMMAND "${CMAKE_SOURCE_DIR}/${DEFAULT_RUNTIME_PATH}/mac/OrxonoxMac")
ENDIF(APPLE)

# Fire up CPack
INCLUDE(CPack)
