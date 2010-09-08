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
	include/enet/callbacks.h
	include/enet/enet.h
	include/enet/list.h
	include/enet/protocol.h
	include/enet/time.h
	include/enet/types.h
	include/enet/unix.h
	include/enet/utility.h
	#include/enet/win32.h

COMPILATION_BEGIN ENetCompilation.cpp
	callbacks.c
	compress.c
	host.c
	list.c
	packet.c
	peer.c
	protocol.c
	unix.c
	#win32.c
COMPILATION_END
)

INCLUDE_DIRECTORIES(include)

ORXONOX_ADD_LIBRARY(enet_orxonox
	ORXONOX_EXTERNAL
	VERSION 1.3.0
	SOURCE_FILES ${ENET_FILES}
)

ADD_DEFINITIONS(-DHAS_POLL -DHAS_FCNTL -DHAS_MSGHDR_FLAGS -DHAS_SOCKLEN_T)
