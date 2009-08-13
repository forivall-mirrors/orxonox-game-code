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
 #    Reto Grieder
 #  Description:
 #    Finds the external media directory and configures the media installation.
 #    Default search paths are: ../../media and ../media_extern
 #

# For dev runs we've got two media directory. The one from the other repository root folder
# is called EXTERNAL_MEDIA
SET(MEDIA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
SET(MEDIA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

# Specify media directory
GET_FILENAME_COMPONENT(_search_path_1 ${CMAKE_SOURCE_DIR}/../${DEFAULT_MEDIA_PATH} ABSOLUTE)
FIND_PATH(EXTERNAL_MEDIA_DIRECTORY resources.oxr
  PATHS
    ${CMAKE_SOURCE_DIR}/media_extern
    ${_search_path_1}
  NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
)

IF(NOT EXTERNAL_MEDIA_DIRECTORY)
  MESSAGE(STATUS "Warning: External media directory not found. If you want to compile while downloading the media files, you will have to recompile about four files afterwards and relink everything. \\n You can specify your own folder with the MEDIA_DIRECTORY variable. Default location for the path is orxonox_root/media_extern")
  # Temporary override to the default location.
  SET(EXTERNAL_MEDIA_DIRECTORY ${CMAKE_SOURCE_DIR}/media_extern)
ENDIF()


################ Installation #################

# Internal directory
INSTALL(
  DIRECTORY ${MEDIA_DIRECTORY}/
  DESTINATION ${MEDIA_INSTALL_DIRECTORY}
  REGEX "\\.svn$|_svn$|CMakeLists.txt|InstallScript.cmake" EXCLUDE
)
# External directory
INSTALL(
  DIRECTORY ${EXTERNAL_MEDIA_DIRECTORY}/
  DESTINATION ${MEDIA_INSTALL_DIRECTORY}
  REGEX "\\.svn$|_svn$|resources\\.oxr" EXCLUDE
)
# Configure the install scripts (variables not available during installation)
CONFIGURE_FILE(MediaInstallScript.cmake ${CMAKE_CURRENT_BINARY_DIR}/MediaInstallScript.cmake @ONLY)
# Join both resources.oxr files
INSTALL(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/MediaInstallScript.cmake)
