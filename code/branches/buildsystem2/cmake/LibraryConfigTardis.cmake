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
 #    Adrian Friedli
 #  Description:
 #    Sets the correct find script paths on the ETH Tardis workstations.
 #    Also sets the TARDIS variable.
 #

IF(UNIX AND NOT APPLE)
  FILE(STRINGS /etc/hostname HOSTNAME LIMIT_COUNT 1)
  IF(${HOSTNAME} MATCHES "^tardis-[a-z][0-9][0-9]$")
    SET (TARDIS ON)
  ENDIF()
ENDIF()

IF(TARDIS)
  MESSAGE(STATUS "Running on D-ITET isg.ee Tardis Computer. Using customized paths.")

  # We cannot use the default compiler (why?)
  SET(CMAKE_C_COMPILER "gcc-4.1")
  SET(CMAKE_CXX_COMPILER "g++-4.1")

  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  SET(BOOST_INCLUDEDIR "/usr/pack/boost-1.34.1-sd/include")
  SET(BOOST_LIBRARYDIR "/usr/pack/boost-1.34.1-sd/i686-debian-linux3.1/lib")
  SET(ENV{CEGUIDIR}    "/usr/pack/cegui-0.5.0-sd;/usr/pack/cegui-0.5.0-sd/i686-debian-linux3.1")
  SET(ENV{ENETDIR}     "/usr/pack/enet-2007-sd;/usr/pack/enet-2007-sd/i686-debian-linux3.1")
  SET(ENV{ALUTDIR}     "/usr/pack/openal-0.0.8-cl;/usr/pack/openal-0.0.8-cl/i686-debian-linux3.1")
  SET(ENV{OGGDIR}      "/usr/pack/oggvorbis-1.0-ds;/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0")
  SET(ENV{VORBISDIR}   "/usr/pack/oggvorbis-1.0-ds;/usr/pack/oggvorbis-1.0-ds/i686-debian-linux3.0")
  SET(ENV{LUA_DIR}     "/usr/pack/lua-5.0.3-sd;/usr/pack/lua-5.0.3-sd/i686-debian-linux3.1")
  SET(ENV{OGRE_HOME}   "/usr/pack/ogre-1.4.5-sd;/usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1")
  SET(ENV{OPENALDIR}   "/usr/pack/openal-0.0.8-cl;/usr/pack/openal-0.0.8-cl/i686-debian-linux3.1")
  #SET(TCL_INCLUDE_PATH "/usr/pack/tcltk-8.4.9.linux-mo/include")
ENDIF(TARDIS)
