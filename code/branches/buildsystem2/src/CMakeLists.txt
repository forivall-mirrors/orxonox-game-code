# Set the search paths for include files
INCLUDE_DIRECTORIES(
  ${OGRE_INCLUDE_DIR}
  ${CEGUI_INCLUDE_DIR}
  ${ENET_INCLUDE_DIR}
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

# Check whether the required CEGUILua version is even available
IF(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ceguilua/ceguilua-${CEGUI_VERSION})
  MESSAGE(FATAL_ERROR "No matching CEGUILua version shipped with Orxonox (${CEGUI_VERSION})")
ENDIF(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ceguilua/ceguilua-${CEGUI_VERSION})

INCLUDE_DIRECTORIES(
  ceguilua/ceguilua-${CEGUI_VERSION}
)

INCLUDE_DIRECTORIES(
  .
  orxonox
  # Required for tolua bind files that are in the build folder
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/orxonox
)

# Set special macro symbols across all libraries
ADD_CXX_FLAGS("-DTIXML_USE_TICPP" 1)
ADD_C_FLAGS  ("-DTIXML_USE_TICPP" 1)

# Include macros
INCLUDE(AddSourceFiles)
INCLUDE(UseTolua)

# External libraries, but copied into the repository
ADD_SUBDIRECTORY(tolua)
ADD_SUBDIRECTORY(ceguilua)
ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(ogreceguirenderer)
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)

# Our own libraries
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(core)
#ADD_SUBDIRECTORY(audio)
ADD_SUBDIRECTORY(network)
ADD_SUBDIRECTORY(orxonox)
