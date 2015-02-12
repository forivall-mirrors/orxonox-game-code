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

INCLUDE(SourceConfig.cmake)

# Configure the two headers and set some options
INCLUDE(OrxonoxConfig.cmake)

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
# Add the icon for win32 builds
# TODO: does this work for mingw too?
ELSEIF(WIN32 AND MSVC)
  LIST(APPEND ORXONOX_MAIN_FILES ${CMAKE_SOURCE_DIR}/contrib/winresource.rc)
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
IF (POLICY CMP0026)
  CMAKE_POLICY(PUSH)
  CMAKE_POLICY(SET CMP0026 OLD) # we only use the file's name, not its actual location, so the old policy is fine
ENDIF()
GET_TARGET_PROPERTY(_exec_loc orxonox-main LOCATION)
GET_FILENAME_COMPONENT(_exec_name ${_exec_loc} NAME)
SET(ORXONOX_EXECUTABLE_NAME ${_exec_name} CACHE INTERNAL "")
IF (POLICY CMP0026)
  CMAKE_POLICY(POP) # set policy back to original settings
ENDIF()


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
