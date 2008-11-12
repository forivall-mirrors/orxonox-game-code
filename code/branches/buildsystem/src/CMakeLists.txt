# Set the search paths for include files
INCLUDE_DIRECTORIES(
  ${OGRE_INCLUDE_DIR}
  ${CEGUI_INCLUDE_DIR}
#  ${CEGUI_OGRE_INCLUDE_DIR}
  ${ENet_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIR}
  ${ALUT_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR}
  ${OGG_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
  ${TCL_INCLUDE_PATH}
  ${DirectX_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
  .
  ois
  orxonox
  tolua
)

ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)
ADD_SUBDIRECTORY(tolua)

# TODO: write include file for this stuff
# Some people may not have CEGUILua installed, espc. with version 0.5
# And we don't know what lua version it was linked against, so we compile
# it ourselves.
# So first, find out what CEGUI version we have.

IF (WIN32)
    ADD_SUBDIRECTORY(ceguilua-0.6.1/ceguilua)
    INCLUDE_DIRECTORIES(ceguilua-0.6.1)
ELSE (WIN32)
    # FIXME
    #IF (${CEGUI_VERSION} LESS 0.6.0)
        ADD_SUBDIRECTORY(ceguilua-0.5.0/ceguilua)
        INCLUDE_DIRECTORIES(ceguilua-0.5.0)
    #ELSE (${CEGUI_VERSION} LESS 0.6.0)
    #    ADD_SUBDIRECTORY(ceguilua-0.6.1/ceguilua)
    #    INCLUDE_DIRECTORIES(ceguilua-0.6.1)
    #ENDIF (${CEGUI_VERSION} LESS 0.6.0)
ENDIF (WIN32)

# Our own libraries
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(audio)
ADD_SUBDIRECTORY(network)
ADD_SUBDIRECTORY(orxonox)
