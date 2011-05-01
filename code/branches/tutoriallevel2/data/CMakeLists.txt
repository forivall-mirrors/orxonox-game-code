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
 #    Finds the external data directory and configures the data installation.
 #    Default search paths are: ../../data and ../data_extern
 #

# For dev runs we've got two data directories. The one from the other repository root folder
# is called EXTERNAL_DATA
SET(DATA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
SET(DATA_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)

# Specify data directory
GET_FILENAME_COMPONENT(_search_path_1 ${CMAKE_SOURCE_DIR}/../data_extern ABSOLUTE)
FIND_PATH(EXTERNAL_DATA_DIRECTORY resources.oxr
  PATHS
    ${CMAKE_SOURCE_DIR}/data_extern
    ${_search_path_1}
  NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
)

IF(NOT EXTERNAL_DATA_DIRECTORY)
  MESSAGE(STATUS "Warning: External data directory not found. If you want to compile while downloading the data files, you will have to recompile about four files afterwards and relink everything.")
  MESSAGE(STATUS "You can specify your own folder with the EXTERNAL_DATA_DIRECTORY variable. Default location for the path is orxonox_root/data_extern")
  # Temporary override to the default location.
  SET(EXTERNAL_DATA_DIRECTORY ${CMAKE_SOURCE_DIR}/data_extern)
ENDIF()


################ Installation #################

# Internal directory
INSTALL(
  DIRECTORY ${DATA_DIRECTORY}/
  DESTINATION ${DATA_INSTALL_DIRECTORY}
  REGEX "\\.svn$|_svn$|CMakeLists.txt|InstallScript.cmake" EXCLUDE
)
# External directory
INSTALL(
  DIRECTORY ${EXTERNAL_DATA_DIRECTORY}/
  DESTINATION ${DATA_INSTALL_DIRECTORY}
  REGEX "\\.svn$|_svn$|AUTHORS|LICENSE" EXCLUDE
)
