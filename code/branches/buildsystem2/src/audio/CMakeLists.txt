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

SET_SOURCE_FILES(AUDIO_FILES
  AudioBuffer.h
  AudioIncludes.h
  AudioManager.h
  AudioPrereqs.h
  AudioSource.h
  AudioStream.h

  AudioBuffer.cc
  AudioManager.cc
  AudioSource.cc
  AudioStream.cc
)
#GET_ALL_HEADER_FILES(AUDIO_HDR_FILES)
#SET(AUDIO_FILES ${AUDIO_SRC_FILES} ${AUDIO_HDR_FILES})
GENERATE_SOURCE_GROUPS(${AUDIO_FILES})

ADD_COMPILER_FLAGS("-w44244" MSVC)

IF (MINGW)
  ADD_LIBRARY( audio ${AUDIO_FILES})
ELSE (MINGW)
  ADD_LIBRARY( audio SHARED ${AUDIO_FILES})
ENDIF (MINGW)

SET_TARGET_PROPERTIES(audio PROPERTIES DEFINE_SYMBOL "AUDIO_SHARED_BUILD")
TARGET_LINK_LIBRARIES( audio
  ${OPENAL_LIBRARY}
  ${ALUT_LIBRARY}
  ${VORBISFILE_LIBRARY}
  ${VORBIS_LIBRARY}
  ${OGG_LIBRARY}
  core
  util
)

IF (NOT WIN32)
  INSTALL(TARGETS audio LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
