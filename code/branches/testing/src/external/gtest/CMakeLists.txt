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

SET_SOURCE_FILES(GTEST_FILES
    src/gtest-all.cc
    include/gtest/gtest.h
)

INCLUDE_DIRECTORIES(
    .
    include
)

ORXONOX_ADD_LIBRARY(gtest_orxonox
    ORXONOX_EXTERNAL
    VERSION         1.6.0
    EXCLUDE_FROM_ALL
    NO_INSTALL
    SOURCE_FILES    ${GTEST_FILES}
)

SET(GTEST_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest_main.cc" CACHE INTERNAL "gtest main() function")
