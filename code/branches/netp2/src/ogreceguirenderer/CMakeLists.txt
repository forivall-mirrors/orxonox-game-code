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

SET(OCR_FILES
  OgreCEGUIRenderer.h
  OgreCEGUIResourceProvider.h
  OgreCEGUITexture.h

  OgreCEGUIRenderer.cpp
  OgreCEGUIResourceProvider.cpp
  OgreCEGUITexture.cpp
)
GENERATE_SOURCE_GROUPS(${OCR_FILES})

# No warnings needed from third party libraries
REMOVE_COMPILER_FLAGS("-W3 -W4" MSVC)
ADD_COMPILER_FLAGS("-w")

ADD_LIBRARY(ogreceguirenderer_orxonox SHARED ${OCR_FILES})
SET_TARGET_PROPERTIES(ogreceguirenderer_orxonox PROPERTIES DEFINE_SYMBOL "OGRE_GUIRENDERER_EXPORTS")
TARGET_LINK_LIBRARIES(ogreceguirenderer_orxonox
  ${OGRE_LIBRARY}
  ${CEGUI_LIBRARY}
)

SET_TARGET_PROPERTIES(ogreceguirenderer_orxonox PROPERTIES VERSION 1.4.9)

ORXONOX_INSTALL(ogreceguirenderer_orxonox)
