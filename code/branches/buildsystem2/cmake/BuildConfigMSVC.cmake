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

#################### Compiler Flags #####################

# -MD    Minimal Rebuild
# -RTC1  Both basic runtime checks
# -MD[d] Multithreaded [debug] DLL
# -Zi    Program Database
# -ZI    Program Database for Edit & Continue
# -WX    General warning Level X
# -wdX   Disable specific warning X
# -wnX   Set warning level of specific warning X to level n

# Overwrite CMake default flags first. Be careful with this
# Only add (not set) the general compiler flags.
# CMake default flags : -DWIN32 -D_WINDOWS -W3 -Zm1000
# additionally for CXX: -EHsc -GR
ADD_COMPILER_FLAGS("-D__WIN32__ -D_WIN32"      CACHE)
ADD_COMPILER_FLAGS("-D_CRT_SECURE_NO_WARNINGS" CACHE)
ADD_COMPILER_FLAGS("-DUNICODE -D_UNICODE"      CACHE)
ADD_COMPILER_FLAGS("-w44522 -w44251 -w44800"   CACHE)

# Increase warning level if requested
IF(EXTRA_WARNINGS)
  REMOVE_COMPILER_FLAGS("-W1 -W2 -W3" CACHE)
  ADD_COMPILER_FLAGS   ("-W4" CACHE)
ELSE(EXTRA_WARNINGS)
  REMOVE_COMPILER_FLAGS("-W1 -W2 -W4" CACHE)
  ADD_COMPILER_FLAGS   ("-W3" CACHE)
ENDIF(EXTRA_WARNINGS) 

# Overwrite CMake default flags here.
SET_COMPILER_FLAGS("-MDd -Od -ZI -D_DEBUG -Gm -RTC1" Debug          CACHE)
SET_COMPILER_FLAGS("-MD  -O2     -DNDEBUG -MP2"      Release        CACHE)
SET_COMPILER_FLAGS("-MD  -O2 -Zi -DNDEBUG"           RelWithDebInfo CACHE)
SET_COMPILER_FLAGS("-MD  -O1     -DNDEBUG -MP2"      MinSizeRel     CACHE)

##################### Linker Flags ######################

# CMake default flags: -MANIFEST -STACK:10000000 -machine:I386
# and INCREMENTAL and DEBUG for debug versions
ADD_LINKER_FLAGS("-OPT:REF -OPT:ICF -OPT:NOWIN98" Release MinSizeRel CACHE)
