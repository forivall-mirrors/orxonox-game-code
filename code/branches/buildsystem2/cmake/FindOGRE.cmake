# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARY, the library to link against to use OGRE.
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright © 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture of the ETH Zurich
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli
# Plugin checking by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(OGRE_INCLUDE_DIR Ogre.h
    PATHS $ENV{OGRE_HOME} /usr/local /usr
    PATH_SUFFIXES include include/OGRE
)
FIND_LIBRARY(OGRE_LIBRARY_OPTIMIZED
    NAMES OgreMain
    PATHS $ENV{OGRE_HOME} /usr/local /usr
    PATH_SUFFIXES lib bin/Release bin/release Release release
)
FIND_LIBRARY(OGRE_LIBRARY_DEBUG
    NAMES OgreMain${LIBRARY_DEBUG_POSTFIX}
    PATHS $ENV{OGRE_HOME} /usr/local /usr
    PATH_SUFFIXES lib bin/Debug bin/debug Debug debug
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGRE DEFAULT_MSG
    OGRE_LIBRARY_OPTIMIZED
    OGRE_INCLUDE_DIR
)

# Set optimized and debug libraries
HandleLibraryTypes(OGRE)

###############################################
# Also make sure we have the required plugins #
###############################################

IF(WIN32)
    # On Windows we need only *.dll, not *.lib. Especially the MSVC generator doesn't look for *.dll
    SET(CMAKE_FIND_LIBRARY_SUFFIXES_STORED ${CMAKE_FIND_LIBRARY_SUFFIXES})
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .dll)
ENDIF(WIN32)
SET(OGRE_RENDER_SYSTEM_FOUND FALSE)
FOREACH(_plugin ${OGRE_PLUGINS})
    FIND_LIBRARY(OGRE_PLUGIN_${_plugin}_OPTIMIZED
        NAMES ${_plugin}
        PATHS $ENV{OGRE_HOME}
        PATH_SUFFIXES lib lib/OGRE bin bin/Release bin/release Release release
    )
    FIND_LIBRARY(OGRE_PLUGIN_${_plugin}_DEBUG
        NAMES ${_plugin}${LIBRARY_DEBUG_POSTFIX}
        PATHS $ENV{OGRE_HOME} /usr/local /usr
        PATH_SUFFIXES lib lib/OGRE bin bin/Debug bin/debug Debug debug
    )
    # We only need at least one render system. Check at the end
    IF(NOT ${_plugin} MATCHES "RenderSystem")
      SET(OGRE_PLUGIN_${_plugin}_FIND_REQUIRED TRUE)
    ELSE(NOT ${_plugin} MATCHES "RenderSystem")
      SET(OGRE_PLUGIN_${_plugin}_FIND_REQUIRED FALSE)
      SET(OGRE_RENDER_SYSTEM_FOUND TRUE)
    ENDIF(NOT ${_plugin} MATCHES "RenderSystem")
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGRE_PLUGIN_${_plugin}
        "Could not find OGRE plugin named ${_plugin}"
        OGRE_PLUGIN_${_plugin}_OPTIMIZED
    )

    # If debug version is not available, release will do as well
    IF(NOT OGRE_PLUGIN_${_plugin}_DEBUG)
        SET(OGRE_PLUGIN_${_plugin}_DEBUG ${OGRE_PLUGIN_${_plugin}_OPTIMIZED} CACHE STRING "")
    ENDIF(NOT OGRE_PLUGIN_${_plugin}_DEBUG)
    MARK_AS_ADVANCED(OGRE_PLUGIN_${_plugin}_OPTIMIZED OGRE_PLUGIN_${_plugin}_DEBUG)

    ### Set variables to configure orxonox.ini correctly afterwards in bin/ ###
    # Check and set the folders
    GET_FILENAME_COMPONENT(_release_folder ${OGRE_PLUGIN_${_plugin}_OPTIMIZED} PATH)
    IF(OGRE_PLUGINS_FOLDER_RELEASE AND NOT OGRE_PLUGINS_FOLDER_RELEASE STREQUAL _release_folder)
        MESSAGE(FATAL_ERROR "Ogre release plugins have to be in the same folder!")
    ENDIF(OGRE_PLUGINS_FOLDER_RELEASE AND NOT OGRE_PLUGINS_FOLDER_RELEASE STREQUAL _release_folder)
    SET(OGRE_PLUGINS_FOLDER_RELEASE ${_release_folder})
    GET_FILENAME_COMPONENT(_debug_folder ${OGRE_PLUGIN_${_plugin}_DEBUG} PATH)
    IF(OGRE_PLUGINS_FOLDER_DEBUG AND NOT OGRE_PLUGINS_FOLDER_DEBUG STREQUAL _debug_folder)
        MESSAGE(FATAL_ERROR "Ogre debug plugins have to be in the same folder!")
    ENDIF(OGRE_PLUGINS_FOLDER_DEBUG AND NOT OGRE_PLUGINS_FOLDER_DEBUG STREQUAL _debug_folder)
    SET(OGRE_PLUGINS_FOLDER_DEBUG ${_debug_folder})

    # Create a list with the plugins for relase and debug configurations
    LIST(APPEND OGRE_PLUGINS_RELEASE ${_plugin})
    IF(OGRE_PLUGIN_${_plugin}_DEBUG)
        LIST(APPEND OGRE_PLUGINS_DEBUG ${_plugin}${LIBRARY_DEBUG_POSTFIX})
    ELSE(OGRE_PLUGIN_${_plugin}_DEBUG)
        LIST(APPEND OGRE_PLUGINS_DEBUG ${_plugin})
    ENDIF(OGRE_PLUGIN_${_plugin}_DEBUG)
ENDFOREACH(_plugin)
IF(NOT OGRE_RENDER_SYSTEM_FOUND)
    MESSAGE(FATAL_ERROR "Could not find an OGRE render system plugin")
ENDIF(NOT OGRE_RENDER_SYSTEM_FOUND)
IF(WIN32)
    # Restore previous settings (see above before FOREACH)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_STORED})
ENDIF(WIN32)

# List has to be comma separated for orxonox.ini
STRING(REPLACE ";" ", " OGRE_PLUGINS_RELEASE "${OGRE_PLUGINS_RELEASE}")
STRING(REPLACE ";" ", " OGRE_PLUGINS_DEBUG   "${OGRE_PLUGINS_DEBUG}")


MARK_AS_ADVANCED(
    OGRE_LIBRARY
    OGRE_LIBRARY_OPTIMIZED
    OGRE_LIBRARY_DEBUG
    OGRE_INCLUDE_DIR
)
