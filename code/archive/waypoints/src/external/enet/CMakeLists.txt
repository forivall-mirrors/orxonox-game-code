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

SET_SOURCE_FILES(ENET_FILES
    callbacks.h
    enet.h
    list.h
    protocol.h
    time.h
    types.h
    unix.h
    utility.h
    win32.h

    callbacks.c
    compress.c
    host.c
    list.c
    packet.c
    peer.c
    protocol.c
    unix.c
    win32.c
)

ADD_COMPILER_FLAGS("-DHAS_POLL -DHAS_FCNTL -DHAS_MSGHDR_FLAGS -DHAS_SOCKLEN_T")

ORXONOX_ADD_LIBRARY(enet_orxonox
    ORXONOX_EXTERNAL
    VERSION         1.3.1
    DEFINE_SYMBOL   "ENET_BUILDING_LIB"
    LINK_LIBS_WIN32 odbc32 odbccp32 winmm ws2_32
    SOURCE_FILES    ${ENET_FILES}
)
