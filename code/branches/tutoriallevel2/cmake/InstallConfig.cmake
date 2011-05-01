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
 #    Configures the installation (paths, rpaths, options)
 #

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) # Variable provided by CMake
  IF("$ENV{ORXONOX_DEV}" OR TARDIS)
    SET(_install_prefix_changed 1)
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH
        "Install path prefix, prepended onto install directories." FORCE)
  ENDIF()
ENDIF()

SET(_info_text "Puts all installed files in subfolders of the install prefix path. That root folder can then be moved, copied and renamed as you wish. The executable will not write to folders like ~/.orxonox or \"Applictation Data\"")
IF(UNIX AND NOT _install_prefix_changed)
  OPTION(INSTALL_COPYABLE "${_info_text}" FALSE)
ELSE()
  OPTION(INSTALL_COPYABLE "${_info_text}" TRUE)
ENDIF()

# Default relative installation paths
SET(RUNTIME_INSTALL_DIRECTORY ${DEFAULT_RUNTIME_PATH})
SET(LIBRARY_INSTALL_DIRECTORY ${DEFAULT_LIBRARY_PATH})
SET(ARCHIVE_INSTALL_DIRECTORY ${DEFAULT_ARCHIVE_PATH})
SET(MODULE_INSTALL_DIRECTORY  ${DEFAULT_MODULE_PATH})
SET(DOC_INSTALL_DIRECTORY     ${DEFAULT_DOC_PATH})
SET(DATA_INSTALL_DIRECTORY    ${DEFAULT_DATA_PATH})
SET(CONFIG_INSTALL_DIRECTORY  ${DEFAULT_CONFIG_PATH})
SET(LOG_INSTALL_DIRECTORY     ${DEFAULT_LOG_PATH})

IF(NOT INSTALL_COPYABLE)
  IF(LINUX)
    # Using absolute paths
    SET(RUNTIME_INSTALL_DIRECTORY games)
    SET(LIBRARY_INSTALL_DIRECTORY lib/games/orxonox)
    SET(ARCHIVE_INSTALL_DIRECTORY lib/games/orxonox/static)
    SET(MODULE_INSTALL_DIRECTORY  lib/games/orxonox/modules)
    SET(DOC_INSTALL_DIRECTORY     share/doc/orxonox)
    SET(DATA_INSTALL_DIRECTORY    share/games/orxonox)
  ELSEIF(WIN32)
    # Leave on default (installs to only one location anyway)
  ELSEIF(APPLE)
    # TODO: Figure out what's the best way to install the application
  ENDIF()

  # Leave empty because it is user and therefore runtime dependent
  SET(CONFIG_INSTALL_DIRECTORY)
  SET(LOG_INSTALL_DIRECTORY)
ENDIF()

################## Unix rpath ###################

# Use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# When building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# The RPATH to be used when installing
IF(INSTALL_COPYABLE)
  # Get relative paths from run to lib and from module to lib directory.
  FILE(RELATIVE_PATH _runtime_rpath "/${RUNTIME_INSTALL_DIRECTORY}" "/${LIBRARY_INSTALL_DIRECTORY}")
  FILE(RELATIVE_PATH _module_rpath  "/${MODULE_INSTALL_DIRECTORY}" "/${LIBRARY_INSTALL_DIRECTORY}")
  # $ORIGIN (with $ escaped) refers to the actual location of the library
  # The UNIX loader recognises this special variable
  SET(RUNTIME_RPATH "\$ORIGIN/${_runtime_rpath}")
  SET(LIBRARY_RPATH "\$ORIGIN")
  SET(MODULE_RPATH  "\$ORIGIN:\$ORIGIN/${_module_rpath}")
ELSE()
  SET(RUNTIME_RPATH "${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DIRECTORY}")
  SET(LIBRARY_RPATH "${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DIRECTORY}")
  SET(MODULE_RPATH  "${LIBRARY_RPATH}:${CMAKE_INSTALL_PREFIX}/${MODULE_INSTALL_DIRECTORY}")
ENDIF()

# Add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
