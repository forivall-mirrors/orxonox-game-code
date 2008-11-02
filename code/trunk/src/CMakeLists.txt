INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(..)
INCLUDE_DIRECTORIES(orxonox)

ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)
ADD_SUBDIRECTORY(lua)
ADD_SUBDIRECTORY(tolua)

# Some people may not have CEGUILua installed, espc. with version 0.5
# And we don't know what lua version it was linked against, so we compile
# it ourselves.
# So first, find out what CEGUI version we have.

IF (WIN32)
    ADD_SUBDIRECTORY(ceguilua-0.6.1/ceguilua)
    INCLUDE_DIRECTORIES(ceguilua-0.6.1)
ELSE (WIN32)
    IF (${CEGUI_VERSION} LESS 0.6.0)
        ADD_SUBDIRECTORY(ceguilua-0.5.0b/ceguilua)
        INCLUDE_DIRECTORIES(ceguilua-0.5.0b)
    ELSE (${CEGUI_VERSION} LESS 0.6.0)
        ADD_SUBDIRECTORY(ceguilua-0.6.1/ceguilua)
        INCLUDE_DIRECTORIES(ceguilua-0.6.1)
    ENDIF (${CEGUI_VERSION} LESS 0.6.0)
ENDIF (WIN32)


# Our own libraries
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(audio)
ADD_SUBDIRECTORY(network)
ADD_SUBDIRECTORY(orxonox)
