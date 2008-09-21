INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(..)
INCLUDE_DIRECTORIES(orxonox)
#INCLUDE_DIRECTORIES(tolua)

ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)

# This section deals with Lua version problems because CEGUILua scripting
# module for CEGUI also uses Lua, but maybe with a different version.
# So first, find out what CEGUI version we have.


ADD_SUBDIRECTORY(tolua)

ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(audio)
ADD_SUBDIRECTORY(network)
ADD_SUBDIRECTORY(orxonox)
