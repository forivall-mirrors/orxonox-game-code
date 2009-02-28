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
 #    Finds the media directory and configures it's installation.
 #

# This directory has to concur with the name set in BuildConfig.cmake
IF(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}/${DEFAULT_MEDIA_PATH}")
  MESSAGE(FATAL_ERROR "Value of DEFAULT_MEDIA_PATH does not concur with the name of the media directory in the source tree")
ENDIF()

# Specify media directory
GET_FILENAME_COMPONENT(_search_path_1 ${CMAKE_SOURCE_DIR}/../media ABSOLUTE)
FIND_PATH(CMAKE_MEDIA_OUTPUT_DIRECTORY resources.cfg
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${_search_path_1}
)
IF(NOT CMAKE_MEDIA_OUTPUT_DIRECTORY)
  MESSAGE(STATUS "Warning: Media directory not found. If you want to compile while downloading the media files, you must specify the directory by Hand BEFORE compiling! Default location is orxonox_root/media")
  # Temporary override to the default location.
  SET(CMAKE_MEDIA_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/media)
  SET(CMAKE_MEDIA_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/media PARENT_SCOPE)
ENDIF()


################ Installation #################

# Try no to copy both tcl script file libraries
IF(TCL_LIBRARY MATCHES "85|8\\.5")
  SET(TCL_EXCLUDE_PATTERN "tcl8\\.4")
ELSEIF(TCL_LIBRARY MATCHES "84|8\\.4")
  SET(TCL_EXCLUDE_PATTERN "tcl8\\.5")
ENDIF()

INSTALL(
  DIRECTORY ${CMAKE_MEDIA_OUTPUT_DIRECTORY}/
  DESTINATION ${ORXONOX_MEDIA_INSTALL_PATH}
  REGEX "\\.svn$|_svn$|backToPNG|${TCL_EXCLUDE_PATTERN}" EXCLUDE
)
