# Find CEGUI includes and library
#
# This module defines
#  CEGUI_INCLUDE_DIR
#  CEGUI_LIBRARY, the library to link against to use CEGUI.
#  CEGUILUA_LIBRARY, the library to link against to use the CEGUI script module.
#  CEGUI_TOLUA_LIBRARY, the library to link against to use Tolua++.
#  CEGUI_FOUND, If false, do not try to use CEGUI
#  CEGUI_VERSION, the version as string "x.y.z"
#
# Input:
#  ENV{CEGUIDIR}, CEGUI path, optional
#
# Created by Matt Williams to find OGRE libraries
# Copyright � 2007, Matt Williams
#
# Modified by Nicolas Schlumberger to find CEGUI libraries
# and make it run on the Tardis-Infrastucture of the ETH Zurich
# Copyright 2007, Nicolas Schlumberger
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli and Reto Grieder
# Version checking and CEGUILua finding by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(CompareVersionStrings)
INCLUDE(DetermineVersion)
INCLUDE(FindPackageHandleAdvancedArgs)
INCLUDE(HandleLibraryTypes)

# Find headers
FIND_PATH(CEGUI_INCLUDE_DIR CEGUI.h
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES include include/CEGUI CEGUI.framework/Headers
)

# Inspect CEGUIVersion.h for the version number
DETERMINE_VERSION(CEGUI ${CEGUI_INCLUDE_DIR}/CEGUIVersion.h)

# Find CEGUI library
FIND_LIBRARY(CEGUI_LIBRARY_OPTIMIZED
  NAMES CEGUIBase CEGUI
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUI_LIBRARY_DEBUG
  NAMES
    CEGUIBased CEGUIBase_d CEGUIBaseD CEGUIBase_D
    CEGUId CEGUI_d CEGUID CEGUI_D
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)

# Find CEGUILua libraries
FIND_LIBRARY(CEGUILUA_LIBRARY_OPTIMIZED
  NAMES CEGUILua CEGUILuaScriptModule
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUILUA_LIBRARY_DEBUG
  NAMES CEGUILuad CEGUILua_d CEGUILuaScriptModuled CEGUILuaScriptModule_d
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)

# Find CEGUI Tolua++ libraries
FIND_LIBRARY(CEGUI_TOLUA_LIBRARY_OPTIMIZED
  NAMES CEGUItoluapp tolua++
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)
FIND_LIBRARY(CEGUI_TOLUA_LIBRARY_DEBUG
  NAMES CEGUItoluappd CEGUItoluapp_d tolua++d tolua++_d
  PATHS $ENV{CEGUIDIR}
  PATH_SUFFIXES lib bin
)

# Newer versions of CEGUI have the renderer for OGRE shipped with them
COMPARE_VERSION_STRINGS("${CEGUI_VERSION}" "0.7" _version_compare TRUE)
IF(_version_compare GREATER -1)
  # Find CEGUI OGRE Renderer libraries
  FIND_LIBRARY(CEGUI_OGRE_RENDERER_LIBRARY_OPTIMIZED
    NAMES CEGUIOgreRenderer
    PATHS $ENV{CEGUIDIR}
    PATH_SUFFIXES lib bin
  )
  FIND_LIBRARY(CEGUI_OGRE_RENDERER_LIBRARY_DEBUG
    NAMES CEGUIOgreRendererd CEGUIOgreRenderer_d
    PATHS $ENV{CEGUIDIR}
    PATH_SUFFIXES lib bin
  )
  SET(CEGUI_OGRE_RENDERER_LIBRARY_NAME CEGUI_OGRE_RENDERER_LIBRARY_OPTIMIZED)
ELSE()
  SET(CEGUI_OGRE_RENDERER_BUILD_REQUIRED TRUE)
ENDIF()

# Handle the REQUIRED argument and set CEGUI_FOUND
# Also checks the version requirements if given
FIND_PACKAGE_HANDLE_ADVANCED_ARGS(CEGUI DEFAULT_MSG "${CEGUI_VERSION}"
  CEGUI_INCLUDE_DIR
  CEGUI_LIBRARY_OPTIMIZED
  CEGUILUA_LIBRARY_OPTIMIZED
  CEGUI_TOLUA_LIBRARY_OPTIMIZED
  ${CEGUI_OGRE_RENDERER_LIBRARY_NAME}
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(CEGUI)
HANDLE_LIBRARY_TYPES(CEGUILUA)
HANDLE_LIBRARY_TYPES(CEGUI_TOLUA)
IF(NOT CEGUI_OGRE_RENDERER_BUILD_REQUIRED)
  HANDLE_LIBRARY_TYPES(CEGUI_OGRE_RENDERER)
ENDIF()

MARK_AS_ADVANCED(
  CEGUI_INCLUDE_DIR
  CEGUI_LIBRARY_OPTIMIZED
  CEGUI_LIBRARY_DEBUG
  CEGUILUA_LIBRARY_OPTIMIZED
  CEGUILUA_LIBRARY_DEBUG
  CEGUI_TOLUA_LIBRARY_OPTIMIZED
  CEGUI_TOLUA_LIBRARY_DEBUG
  CEGUI_OGRE_RENDERER_LIBRARY_OPTIMIZED
  CEGUI_OGRE_RENDERER_LIBRARY_DEBUG
)