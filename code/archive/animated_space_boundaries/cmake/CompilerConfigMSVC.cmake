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
 #    Sets the right compiler and linker flags for the Microsoft Compiler.
 #

INCLUDE(FlagUtilities)

# We make use of variadic macros, which is only supported by MSVC 8 and above
IF(MSVC_VERSION LESS 1400)
  MESSAGE(FATAL_ERROR "Microsoft Visual Studio versions below 8 (2005) are not supported.")
ENDIF()

# Orxonox only supports MSVC 8 and above, which gets asserted above
SET(PCH_COMPILER_SUPPORT TRUE)


#################### Compiler Flags #####################

# CMake default flags : -DWIN32 -D_WINDOWS -W3 -Zm1000
# additionally for CXX: -EHsc -GR
# We keep these flags but reset the build specific flags
SET_COMPILER_FLAGS("" Debug RelForDevs RelWithDebInfo Release MinSizeRel CACHE)

# Make sure we define all the possible macros for identifying Windows
ADD_COMPILER_FLAGS("-D__WIN32__ -D_WIN32"  CACHE)
# Suppress some annoying warnings
ADD_COMPILER_FLAGS("-D_CRT_SECURE_NO_WARNINGS" CACHE)
ADD_COMPILER_FLAGS("-D_SCL_SECURE_NO_WARNINGS" CACHE)

# Use multiprocessed compiling (like "make -j3" on Unix)
ADD_COMPILER_FLAGS("-MP" CACHE)

# Never omit frame pointers to avoid useless stack traces (the performance
# loss is almost not measurable)
ADD_COMPILER_FLAGS("-Oy-" CACHE)
# Enable non standard floating point optimisations
ADD_COMPILER_FLAGS("-fp:fast" CACHE)

# Set build specific flags.
# -MD[d]    Multithreaded [debug] shared MSVC runtime library
# -Zi       Generate debug symbols
# -O[d|2|1] No optimisations, optimise for speed, optimise for size
# -Oi[-]    Use or disable use of intrinisic functions
# -GL       Link time code generation (see -LTCG in linker flags)
# -RTC1     Both basic runtime checks
ADD_COMPILER_FLAGS("-MDd -Zi -Od -Oi  -D_DEBUG -RTC1" Debug          CACHE)
ADD_COMPILER_FLAGS("-MD  -Zi -O2 -Oi  -DNDEBUG"       RelForDevs     CACHE)
ADD_COMPILER_FLAGS("-MD  -Zi -O2 -Oi  -DNDEBUG -GL"   RelWithDebInfo CACHE)
ADD_COMPILER_FLAGS("-MD      -O2 -Oi  -DNDEBUG -GL"   Release        CACHE)
ADD_COMPILER_FLAGS("-MD      -O1 -Oi- -DNDEBUG -GL"   MinSizeRel     CACHE)


####################### Warnings ########################

# -WX    General warning Level X
# -wdX   Disable specific warning X
# -wnX   Set warning level of specific warning X to level n

# Increase warning level if requested
IF(EXTRA_COMPILER_WARNINGS)
  REMOVE_COMPILER_FLAGS("-W1 -W2 -W3" CACHE)
  ADD_COMPILER_FLAGS   ("-W4" CACHE)
ELSE()
  REMOVE_COMPILER_FLAGS("-W1 -W2 -W4" CACHE)
  ADD_COMPILER_FLAGS   ("-W3" CACHE)
ENDIF()

# "<type> needs to have dll-interface to be used by clients'
# Happens on STL member variables which are not public
ADD_COMPILER_FLAGS("-w44251" CACHE)
ADD_COMPILER_FLAGS("-w44275" CACHE) # For inheritance

# Multiple assignment operators specified
ADD_COMPILER_FLAGS("-w44522" CACHE)

# Forcing values to bool
ADD_COMPILER_FLAGS("-w44800" CACHE)

# TODO: Resolve the cause of this warning!
# ('class1' : inherits 'class2::member' via dominance)
ADD_COMPILER_FLAGS("-w44250" CACHE)

# conversion from 'double' to 'float', possible loss of data
# conversion from 'ogg_int64_t' to 'long', possible loss of data
# ADD_COMPILER_FLAGS("-w44244" CACHE)

# "conversion from 'size_t' to 'unsigned int', possible loss of data
# ADD_COMPILER_FLAGS("-w44267" CACHE)

# "truncation from 'double' to 'float'
# ADD_COMPILER_FLAGS("-w44305" CACHE)

# "non dll-interface class used as base for dll-interface class"
# ADD_COMPILER_FLAGS("-w44275" CACHE)


##################### Linker Flags ######################

# CMake default flags: -MANIFEST -STACK:10000000 -machine:I386
# We keep these flags but reset the build specific flags
SET_LINKER_FLAGS("" Debug RelForDevs RelWithDebInfo Release MinSizeRel CACHE)

# Never fold multiple functions into a single one because we might compare
# function pointers (for instance with network functions)
ADD_LINKER_FLAGS("-OPT:NOICF" CACHE)

# Very old flag that would do some extra Windows 98 alignment optimisations
ADD_LINKER_FLAGS("-OPT:NOWIN98" MSVC80 CACHE)

# Generate debug symbols
ADD_LINKER_FLAGS("-DEBUG" Debug RelForDevs RelWithDebInfo CACHE)

# Incremental linking speeds up development builds
ADD_LINKER_FLAGS("-INCREMENTAL:YES" Debug   RelForDevs                CACHE)
ADD_LINKER_FLAGS("-INCREMENTAL:NO"  Release RelWithDebInfo MinSizeRel CACHE)

# Eliminate unreferenced data
ADD_LINKER_FLAGS("-OPT:REF" Release RelWithDebInfo MinSizeRel CACHE)

# Link time code generation can improve run time performance at the cost of
# hugely increased link time (the total build time is about the same though)
ADD_LINKER_FLAGS("-LTCG" Release RelWithDebInfo MinSizeRel CACHE)
