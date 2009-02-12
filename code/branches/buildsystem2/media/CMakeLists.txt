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

# Specify media directory
FIND_PATH(ORXONOX_MEDIA_DIRECTORY resources.cfg
  PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/../media
	$ENV{ORXONOX_MEDIA_DIR}
)
IF(NOT ORXONOX_MEDIA_DIRECTORY)
  MESSAGE(STATUS "Warning: Media directory not found. If you want to compile while downloading the media files, you must specify the directory by Hand BEFORE compiling! Default location is orxonox_root/media")
  # Temporary override to the default location.
  SET(ORXONOX_MEDIA_DIRECTORY ${CMAKE_SOURCE_DIR}/media)
ENDIF()


################ Installation #################

IF(UNIX)
  SET(MEDIA_INSTALL_SUFFIX "share/orxonox")
ELSEIF(WIN32)
  SET(MEDIA_INSTALL_SUFFIX "media")
ELSE()
  MESSAGE(FATAL_ERROR "Install location for media files not yet specified for this platform. Please Adjust root/media/CMakeLists.txt")
ENDIF()

INSTALL(DIRECTORY ${ORXONOX_MEDIA_DIRECTORY}/ DESTINATION
        ${MEDIA_INSTALL_SUFFIX} REGEX "\\.svn$|_svn$" EXCLUDE)
