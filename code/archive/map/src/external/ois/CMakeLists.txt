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

SET_SOURCE_FILES(OIS_FILES
  OIS.h
  OISConfig.h
  OISEffect.h
  OISEvents.h
  OISException.h
  OISFactoryCreator.h
  OISForceFeedback.h
  OISInputManager.h
  OISInterface.h
  OISJoyStick.h
  OISKeyboard.h
  OISMouse.h
  OISObject.h
  OISPrereqs.h

COMPILATION_BEGIN OISCompilation.cpp
  OISEffect.cpp
  OISException.cpp
  OISForceFeedback.cpp
  OISInputManager.cpp
  OISJoyStick.cpp
  OISKeyboard.cpp
  OISObject.cpp
COMPILATION_END
)
IF(WIN32)
  ADD_SUBDIRECTORY(win32)
ELSEIF(APPLE)
  ADD_SUBDIRECTORY(mac)
ELSEIF(UNIX)
  ADD_SUBDIRECTORY(linux)
ENDIF()

INCLUDE_DIRECTORIES(.)

ORXONOX_ADD_LIBRARY(ois_orxonox
  ORXONOX_EXTERNAL
  DEFINE_SYMBOL
    "OIS_NONCLIENT_BUILD"
  VERSION
    1.2
  SOURCE_FILES
    ${OIS_FILES}
)

IF(WIN32)
  TARGET_LINK_LIBRARIES(ois_orxonox ${DIRECTX_LIBRARIES})
ENDIF()
