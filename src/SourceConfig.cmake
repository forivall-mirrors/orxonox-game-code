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
 #    Configures the compilers and sets build options.
 #

# Required macros and functions
INCLUDE(FlagUtilities)

####### Library Behaviour (dependencies) ########

# Disable auto linking completely for Boost and POCO
ADD_COMPILER_FLAGS("-DBOOST_ALL_NO_LIB")
ADD_COMPILER_FLAGS("-DPOCO_NO_AUTOMATIC_LIBS")

IF(WIN32)
  # If no defines are specified, these libs get linked statically
  ADD_COMPILER_FLAGS("-DBOOST_ALL_DYN_LINK" LINK_BOOST_DYNAMIC)
  #ADD_COMPILER_FLAGS("-DENET_DLL"           LINK_ENET_DYNAMIC)
  ADD_COMPILER_FLAGS("-DLUA_BUILD_AS_DLL"   LINK_LUA5.1_DYNAMIC)
  # If no defines are specified, these libs get linked dynamically
  ADD_COMPILER_FLAGS("-DCEGUI_STATIC -DTOLUA_STATIC" NOT LINK_CEGUI_DYNAMIC)
  ADD_COMPILER_FLAGS("-DOGRE_STATIC_LIB"    NOT LINK_OGRE_DYNAMIC)
  ADD_COMPILER_FLAGS("-DSTATIC_BUILD"       NOT LINK_TCL_DYNAMIC)

  # Target Windows XP as minimum Windows version
  # And try to catch all the different macro defines for that...
  IF(MINGW)
    ADD_COMPILER_FLAGS("-D_WIN32_WINNT=0x05010000")
    ADD_COMPILER_FLAGS("-D_WIN32_WINDOWS=0x05010000")
    ADD_COMPILER_FLAGS("-DWINVER=0x05010000")
    ADD_COMPILER_FLAGS("-DNTDDI_VERSION=0x05010000")
  ELSE()
    ADD_COMPILER_FLAGS("-D_WIN32_WINNT=0x0501")
    ADD_COMPILER_FLAGS("-D_WIN32_WINDOWS=0x0501")
    ADD_COMPILER_FLAGS("-DWINVER=0x0501")
    ADD_COMPILER_FLAGS("-DNTDDI_VERSION=0x05010000")
  ENDIF()
ENDIF(WIN32)

# Visual Leak Dectector configuration
IF(MSVC AND VLD_FOUND)
  OPTION(VISUAL_LEAK_DETECTOR_ENABLE "Memory leak detector" OFF)
  IF(VISUAL_LEAK_DETECTOR_ENABLE)
    # Force library linking by forcing the inclusion of a symbol
    ADD_LINKER_FLAGS("-INCLUDE:__imp_?g_vld@@3VVisualLeakDetector@@A" Debug)
    IF(MSVC90)
      # VS 2008 incremental linker crashes with /INCLUDE most of the time
      REMOVE_LINKER_FLAGS("-INCREMENTAL:YES" Debug)
      ADD_LINKER_FLAGS   ("-INCREMENTAL:NO"  Debug)
    ENDIF()
  ENDIF()
ENDIF()

######### Library Behaviour (external) ##########

# Use TinyXML++
ADD_COMPILER_FLAGS("-DTIXML_USE_TICPP")

# Default linking for externals
IF(CMAKE_BUILD_TYPE MATCHES "(Debug|RelForDevs)")
  SET(_default_link_mode "SHARED")
ELSE()
  SET(_default_link_mode "STATIC")
ENDIF()
SET(_message "Link mode for external libraries that we build ourselves.
MSVC Note: certain libraries will not be linked shared.")
SET(ORXONOX_EXTERNAL_LINK_MODE "${_default_link_mode}" CACHE STRING "${_message}")

IF(ORXONOX_EXTERNAL_LINK_MODE STREQUAL "SHARED")
  SET(_external_shared_link TRUE)
ELSE()
  SET(_external_shared_link FALSE)
ENDIF()
# If no defines are specified, these libs get linked dynamically
ADD_COMPILER_FLAGS("-DENET_DLL"                    WIN32     _external_shared_link)
ADD_COMPILER_FLAGS("-DOGRE_GUIRENDERER_STATIC_LIB" WIN32 NOT _external_shared_link)
ADD_COMPILER_FLAGS("-DOIS_STATIC_LIB"              WIN32 NOT _external_shared_link)

############## Include Directories ##############

# Set the search paths for include files
INCLUDE_DIRECTORIES(
  # OrxonoxConfig.h
  ${CMAKE_CURRENT_BINARY_DIR}

  # All includes in "externals" should be prefixed with the path
  # relative to "external" to avoid conflicts
  ../src/external
  # Include directories needed even if only included by Orxonox
  ../src/external/bullet
  ../src/external/ois

  # External
  ${OGRE_INCLUDE_DIR}
  ${CEGUI_INCLUDE_DIR}
  ${CEGUI_TOLUA_INCLUDE_DIR}
  #${ENET_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${POCO_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIRS}
  ${ALUT_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR}
  ${OGG_INCLUDE_DIR}
  ${LUA5.1_INCLUDE_DIR}
  ${TCL_INCLUDE_PATH}
  ${DIRECTX_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
)

IF(CEGUI_OLD_VERSION)
  INCLUDE_DIRECTORIES(${CEGUILUA_INCLUDE_DIR})
ENDIF()

IF (DBGHELP_FOUND)
  INCLUDE_DIRECTORIES(${DBGHELP_INCLUDE_DIR})
ENDIF()

############## CEGUI OGRE Renderer ##############

IF(CEGUI_OGRE_RENDERER_BUILD_REQUIRED)
  SET(CEGUI_OGRE_RENDERER_LIBRARY ogreceguirenderer_orxonox)
ENDIF()

################### Tolua Bind ##################

# Add hook script to the lua code that generates the bindings
SET(TOLUA_PARSER_HOOK_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/libraries/core/ToluaInterfaceHook.lua)
SET(TOLUA_PARSER_DEPENDENCIES ${TOLUA_PARSER_DEPENDENCIES} ${TOLUA_PARSER_HOOK_SCRIPT})
