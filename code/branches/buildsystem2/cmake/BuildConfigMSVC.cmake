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
 
###################### MSVC config ########################
# Set the library directories and special options when
# using Visual Studio.
###########################################################

IF (MSVC)

  #################### Compiler Flags #####################

  # /MD    Minimal Rebuild
  # /RTC1  Both basic runtime checks
  # /MD[d] Multithreaded [debug] DLL
  # /Zi    Program Database
  # /ZI    Program Database for Edit & Continue
  # /WX    Warning Level X
  # /wdX   Disable specific warning X
  SET(MSVC_CL_FLAGS "
    /D WIN32 /D __WIN32__ /D _WIN32 /D _WINDOWS
    /D BOOST_ALL_DYN_LINK
    /D OIS_DYNAMIC_LIB
    /D ZLIB_WINAPI
    /D LUA_BUILD_AS_DLL
    /D _CRT_SECURE_NO_WARNINGS
    /W3
    /EHsc
    /wd4522
    /wd4251
    /wd4800
  ")
  SET(CMAKE_C_FLAGS                  "${MSVC_CL_FLAGS}")
  SET(CMAKE_CXX_FLAGS                "${MSVC_CL_FLAGS}")

  # Note: ${CMAKE_C_FLAGS} get added to the specific ones
  SET(MSVC_CL_FLAGS_DEBUG            "/MDd /Od  /Zi /Gm /RTC1")
  SET(MSVC_CL_FLAGS_RELEASE          "/MD  /MP2 /D TOLUA_RELEASE")
  SET(CMAKE_C_FLAGS_DEBUG            "${MSVC_CL_FLAGS_DEBUG}")
  SET(CMAKE_CXX_FLAGS_DEBUG          "${MSVC_CL_FLAGS_DEBUG}")
  SET(CMAKE_C_FLAGS_RELEASE          "${MSVC_CL_FLAGS_RELEASE} /O2")
  SET(CMAKE_CXX_FLAGS_RELEASE        "${MSVC_CL_FLAGS_RELEASE} /O2")
  SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "${MSVC_CL_FLAGS_RELEASE} /O2 /Zi")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${MSVC_CL_FLAGS_RELEASE} /O2 /Zi")
  SET(CMAKE_C_FLAGS_MINSIZEREL       "${MSVC_CL_FLAGS_RELEASE} /O1")
  SET(CMAKE_CXX_FLAGS_MINSIZEREL     "${MSVC_CL_FLAGS_RELEASE} /O1")

  ##################### Linker Flags ######################

  SET(MSVC_LINKER_FLAGS                        "")
  SET(CMAKE_EXE_LINKER_FLAGS                   "${MSVC_LINKER_FLAGS}")
  SET(CMAKE_SHARED_LINKER_FLAGS                "${MSVC_LINKER_FLAGS}")

  # Note: ${CMAKE_EXE_LINKER_FLAGS} get added to the specific ones
  SET(MSVC_LINKER_FLAGS_DEBUG                  "/INCREMENTAL:YES")
  SET(MSVC_LINKER_FLAGS_RELEASE                "/INCREMENTAL:NO /OPT:REF /OPT:ICF")
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG             "${MSVC_LINKER_FLAGS_DEBUG}")
  SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG          "${MSVC_LINKER_FLAGS_DEBUG}")
  SET(CMAKE_EXE_LINKER_FLAGS_RELEASE           "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE        "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO    "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL        "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL     "${MSVC_LINKER_FLAGS_RELEASE}")

ENDIF (MSVC)
