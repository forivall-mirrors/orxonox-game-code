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
INCLUDE(TargetUtilities)

# Configure the two headers and set some options
INCLUDE(OrxonoxConfig.cmake)

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
  ADD_COMPILER_FLAGS("-D_WIN32_WINNT=0x0501")
  ADD_COMPILER_FLAGS("-D_WIN32_WINDOWS=0x0501")
  ADD_COMPILER_FLAGS("-DWINVER=0x0501")
  ADD_COMPILER_FLAGS("-DNTDDI_VERSION=0x05010000")
ENDIF(WIN32)

# Visual Leak Dectector configuration
IF(MSVC AND VLD_FOUND)
  OPTION(VISUAL_LEAK_DETECTOR_ENABLE "Memory leak detector" OFF)
  IF(VISUAL_LEAK_DETECTOR_ENABLE)
    # Force library linking by forcing the inclusion of a symbol
    ADD_LINKER_FLAGS("-INCLUDE:__imp_?vld@@3VVisualLeakDetector@@A" Debug)
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
  ${CMAKE_CURRENT_SOURCE_DIR}/external
  # Include directories needed even if only included by Orxonox
  ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet
  ${CMAKE_CURRENT_SOURCE_DIR}/external/ois

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

IF(VISUAL_LEAK_DETECTOR_ENABLE)
  INCLUDE_DIRECTORIES(${VLD_INCLUDE_DIR})
ENDIF()

############## CEGUI OGRE Renderer ##############

IF(CEGUI_OGRE_RENDERER_BUILD_REQUIRED)
  SET(CEGUI_OGRE_RENDERER_LIBRARY ogreceguirenderer_orxonox)
ENDIF()

################### Tolua Bind ##################

# Add hook script to the lua code that generates the bindings
SET(TOLUA_PARSER_HOOK_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/libraries/core/ToluaInterfaceHook.lua)
SET(TOLUA_PARSER_DEPENDENCIES ${TOLUA_PARSER_DEPENDENCIES} ${TOLUA_PARSER_HOOK_SCRIPT})

################ Sub Directories ################

ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(libraries)
ADD_SUBDIRECTORY(orxonox)
SET(ORXONOX_MODULES CACHE INTERNAL "")
ADD_SUBDIRECTORY(modules)

################## Executable ###################

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/libraries
  ${CMAKE_CURRENT_SOURCE_DIR}/orxonox
)

# Translate argument
IF(ORXONOX_USE_WINMAIN)
  SET(ORXONOX_WIN32 WIN32)
ENDIF()

SET(ORXONOX_MAIN_FILES Orxonox.cc)

# Add special source file for OS X
IF(APPLE)
  LIST(APPEND ORXONOX_MAIN_FILES OrxonoxMac.mm)
ENDIF()

ORXONOX_ADD_EXECUTABLE(orxonox-main
  # When defined as WIN32 this removes the console window on Windows
  ${ORXONOX_WIN32}
  LINK_LIBRARIES
    orxonox
  SOURCE_FILES
    ${ORXONOX_MAIN_FILES}
  OUTPUT_NAME orxonox
)
# Main executable should depend on all modules
ADD_DEPENDENCIES(orxonox-main ${ORXONOX_MODULES})

# Get name to configure the run scripts
GET_TARGET_PROPERTY(_exec_loc orxonox-main LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE INTERNAL "")


# When using Visual Studio we want to use the output directory as working
# directory and we also want to specify where the external dlls
# (lua, ogre, etc.) are. The problem hereby is that these information cannot
# be specified in CMake because they are not stored in the actual project file.
# This workaround will create a configured *.vcproj.user file that holds the
# right values. When starting the solution for the first time,
# these get written to the *vcproj.yourPCname.yourname.user
IF(MSVC)
  IF(CMAKE_CL_64)
    SET(MSVC_PLATFORM "x64")
  ELSE()
    SET(MSVC_PLATFORM "Win32")
  ENDIF()
  IF(MSVC10)
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox-main.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/orxonox-main.vcxproj.user")
  ELSE()
    STRING(REGEX REPLACE "^Visual Studio ([0-9][0-9]?).*$" "\\1"
           VISUAL_STUDIO_VERSION_SIMPLE "${CMAKE_GENERATOR}")
    CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/orxonox-main.vcproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/orxonox-main.vcproj.user")
  ENDIF()
ENDIF(MSVC)

# Apple Mac OS X specific build settings
IF(APPLE)
  # On Apple we need to link to AppKit and Foundation frameworks
  TARGET_LINK_LIBRARIES(orxonox-main 
    "-framework AppKit"
    "-framework Foundation"
  )

  # Post-build step for the creation of the Dev-App bundle
  INCLUDE(PrepareDevBundle)
  ADD_CUSTOM_COMMAND(
    TARGET orxonox-main
    POST_BUILD 
    # Copy the executable into the Orxonox.app
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/${ORXONOX_EXECUTABLE_NAME}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}.app/Contents/MacOS"
    # Copy the dev-build marker file to Orxonox.app
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/orxonox_dev_build.keep_me" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}.app/Contents/MacOS"
    # Create a shortcut of the application to the root of the build tree
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}.app" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app" 
  ) 
ENDIF(APPLE)

#################### Doxygen ####################

# Prepare include paths for Doxygen. This is necessary to display
# the correct path to use when including a file, e.g.
# core/XMLPort.h instead of src/core/XMLPort.h

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/modules)
GET_DIRECTORY_PROPERTY(_temp INCLUDE_DIRECTORIES)
# Replace ';' by spaces
STRING(REPLACE ";" " " _temp "${_temp}")
SET(DOXYGEN_INCLUDE_DIRECTORIES "${_temp}" PARENT_SCOPE)
