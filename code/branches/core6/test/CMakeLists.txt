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
 #    Fabian 'x3n' Landau
 #  Description:
 #    Configures the compilers and sets build options.
 #

# Required macros and functions
INCLUDE(FlagUtilities)
INCLUDE(TargetUtilities)

INCLUDE(../src/SourceConfig.cmake)

################ Testing Configuration ################

ENABLE_TESTING()

ADD_CUSTOM_TARGET(test COMMAND ${CMAKE_CTEST_COMMAND} "$(ARGS)" COMMENT "Running tests")

CONFIGURE_FILE("CTestCustom.cmake" "CTestCustom.cmake" @ONLY)

############## Include Directories ##############

INCLUDE_DIRECTORIES(
  ../src/external
  ../src/libraries
  ../src/orxonox
  ../src/modules
  ../src/external/gtest/include
  ../src/external/gmock/include
  ${CMAKE_BINARY_DIR}/src
)

################ Sub Directories ################

ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(core)
