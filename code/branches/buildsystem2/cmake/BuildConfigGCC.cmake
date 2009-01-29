 #
 #   ORXONOX - the hottest 3D action shooter ever to exist
 #                    > www.orxonox.net <
 #
 #
 #   License notice:
 #
 #   This program is free software; you can redistribute it and/or
 #   modify it under the terms of the GNU General Public License
 #   as published by the Free Software Foundation; either version 2
 #   of the License, or (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program; if not, write to the Free Software
 #   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #   Author:
 #      Reto Grieder
 #   Co-authors:
 #      ...
 #

###### Default Compiler/Linker Options ##########
# Most people use GCC to compile orxonox, so use that as default

SET(CMAKE_C_FLAGS   "$ENV{CFLAGS}   ${ORXONOX_WARNING_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${ORXONOX_WARNING_FLAGS} -fPIC")
# These flags are added to the flags above
SET(CMAKE_C_FLAGS_DEBUG            "    -g -ggdb")
SET(CMAKE_CXX_FLAGS_DEBUG          "    -g -ggdb")
SET(CMAKE_C_FLAGS_RELEASE          "-O3          -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE        "-O3          -DNDEBUG")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g -ggdb -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ggdb -DNDEBUG")
SET(CMAKE_C_FLAGS_MINSIZEREL       "-Os          -DNDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os          -DNDEBUG")

SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS    " --no-undefined")
SET(CMAKE_SHARED_LINKER_FLAGS " --no-undefined")
SET(CMAKE_MODULE_LINKER_FLAGS " --no-undefined")
