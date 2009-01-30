#    Author: Reto '1337' Grieder (2008)
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

###################################################
# Make sure we have the required plugins for OGRE #
###################################################

FUNCTION(CHECK_OGRE_PLUGINS)

  SET(OGRE_PLUGINS ${ARGN})

  IF(WIN32)
    # On Windows we need only *.dll, not *.lib. Especially the MSVC generator doesn't look for *.dll
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .dll)
  ENDIF(WIN32)
  # Do not prefix "lib" on any platform
  SET(CMAKE_FIND_LIBRARY_PREFIXES "")

  SET(OGRE_RENDER_SYSTEM_FOUND FALSE)
  FOREACH(_plugin ${OGRE_PLUGINS})
    FIND_LIBRARY(OGRE_PLUGIN_${_plugin}_OPTIMIZED
      NAMES ${_plugin}
      PATHS $ENV{OGRE_HOME} $ENV{OGRE_PLUGIN_DIR}
      PATH_SUFFIXES lib lib/OGRE bin bin/Release bin/release Release release
    )
    FIND_LIBRARY(OGRE_PLUGIN_${_plugin}_DEBUG
      NAMES ${_plugin}d ${_plugin}_d
      PATHS $ENV{OGRE_HOME} $ENV{OGRE_PLUGIN_DIR}
      PATH_SUFFIXES lib lib/OGRE bin bin/Debug bin/debug Debug debug
    )
    # We only need at least one render system. Check at the end.
    IF(NOT ${_plugin} MATCHES "RenderSystem")
      IF(NOT OGRE_PLUGIN_${_plugin}_OPTIMIZED)
        MESSAGE(FATAL_ERROR "Could not find OGRE plugin named ${_plugin}")
      ENDIF()
    ELSEIF(OGRE_PLUGIN_${_plugin}_OPTIMIZED)
      SET(OGRE_RENDER_SYSTEM_FOUND TRUE)
    ENDIF()

    IF(OGRE_PLUGIN_${_plugin}_OPTIMIZED)
      # If debug version is not available, release will do as well
      IF(NOT OGRE_PLUGIN_${_plugin}_DEBUG)
        SET(OGRE_PLUGIN_${_plugin}_DEBUG ${OGRE_PLUGIN_${_plugin}_OPTIMIZED} CACHE STRING "" FORCE)
      ENDIF()
      MARK_AS_ADVANCED(OGRE_PLUGIN_${_plugin}_OPTIMIZED OGRE_PLUGIN_${_plugin}_DEBUG)

      ### Set variables to configure orxonox.ini correctly afterwards in bin/ ###
      # Check and set the folders
      GET_FILENAME_COMPONENT(_release_folder ${OGRE_PLUGIN_${_plugin}_OPTIMIZED} PATH)
      IF(OGRE_PLUGINS_FOLDER_RELEASE AND NOT OGRE_PLUGINS_FOLDER_RELEASE STREQUAL _release_folder)
        MESSAGE(FATAL_ERROR "Ogre release plugins have to be in the same folder!")
      ENDIF()
      SET(OGRE_PLUGINS_FOLDER_RELEASE ${_release_folder})
      GET_FILENAME_COMPONENT(_debug_folder ${OGRE_PLUGIN_${_plugin}_DEBUG} PATH)
      IF(OGRE_PLUGINS_FOLDER_DEBUG AND NOT OGRE_PLUGINS_FOLDER_DEBUG STREQUAL _debug_folder)
        MESSAGE(FATAL_ERROR "Ogre debug plugins have to be in the same folder!")
      ENDIF()
      SET(OGRE_PLUGINS_FOLDER_DEBUG ${_debug_folder})

      # Create a list with the plugins for relase and debug configurations
      LIST(APPEND OGRE_PLUGINS_RELEASE ${_plugin})
      IF(OGRE_PLUGIN_${_plugin}_DEBUG)
        # Determine debug postfix ("d" or "_d")
        IF(OGRE_PLUGIN_${_plugin}_DEBUG MATCHES "_d\\.|_d$")
          LIST(APPEND OGRE_PLUGINS_DEBUG "${_plugin}_d")
        ELSE()
          LIST(APPEND OGRE_PLUGINS_DEBUG "${_plugin}d")
        ENDIF()
      ELSE()
        LIST(APPEND OGRE_PLUGINS_DEBUG ${_plugin})
      ENDIF()
    ENDIF(OGRE_PLUGIN_${_plugin}_OPTIMIZED)
  ENDFOREACH(_plugin)
  IF(NOT OGRE_RENDER_SYSTEM_FOUND)
      MESSAGE(FATAL_ERROR "Could not find an OGRE render system plugin")
  ENDIF()

  # List has to be comma separated for orxonox.ini
  STRING(REPLACE ";" ", " OGRE_PLUGINS_RELEASE "${OGRE_PLUGINS_RELEASE}")
  STRING(REPLACE ";" ", " OGRE_PLUGINS_DEBUG   "${OGRE_PLUGINS_DEBUG}")

  # Set variables outside function scope
  SET(OGRE_PLUGINS_FOLDER_DEBUG ${OGRE_PLUGINS_FOLDER_DEBUG} PARENT_SCOPE)
  SET(OGRE_PLUGINS_FOLDER_RELEASE ${OGRE_PLUGINS_FOLDER_RELEASE} PARENT_SCOPE)
  SET(OGRE_PLUGINS_RELEASE ${OGRE_PLUGINS_RELEASE} PARENT_SCOPE)
  SET(OGRE_PLUGINS_DEBUG ${OGRE_PLUGINS_DEBUG} PARENT_SCOPE)

ENDFUNCTION(CHECK_OGRE_PLUGINS)
