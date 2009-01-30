# various macro includes
INCLUDE(FlagUtilities)
INCLUDE(SourceFileUtilities)
INCLUDE(GenerateToluaBindings)

# Use TinyXML++
ADD_COMPILER_FLAGS("-DTIXML_USE_TICPP")
# OIS dynamic linking requires macro definition, at least for Windows
ADD_COMPILER_FLAGS("-DOIS_DYNAMIC_LIB")
# Tolua binding speedup if required
ADD_COMPILER_FLAGS("-DTOLUA_RELEASE" Release MinSizeRel TOLUA_PARSER_RELEASE)

################ OrxonoxConfig.h ################

# Copy and configure OrxonoxConfig which gets included in every file
CONFIGURE_FILE(OrxonoxConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/OrxonoxConfig.h)


############## Include Directories ##############

# Set the search paths for include files
INCLUDE_DIRECTORIES(
  # External
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
  ${DIRECTX_INCLUDE_DIR}
  ${ZLIB_INCLUDE_DIR}

  # Internal
  .
  orxonox
  # Required for tolua bind and config files that are in the binary folder
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/orxonox
)


################ Sub Directories ################

# Third party libraries
ADD_SUBDIRECTORY(tolua)

# Include CEGUILua if not requested otherwise
IF(CEGUILUA_USE_INTERNAL_LIBRARY)
  IF(NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ceguilua/ceguilua-${CEGUI_VERSION})
    MESSAGE(FATAL_ERROR "CEGUILua version not found in src folder. Update list of supported versions in LibraryConfig.cmake!")
  ENDIF()

  INCLUDE_DIRECTORIES(ceguilua/ceguilua-${CEGUI_VERSION})
  ADD_SUBDIRECTORY(ceguilua)
ENDIF()

ADD_SUBDIRECTORY(cpptcl)
ADD_SUBDIRECTORY(ogreceguirenderer)
ADD_SUBDIRECTORY(ois)
ADD_SUBDIRECTORY(tinyxml)

# Orxonox code
ADD_SUBDIRECTORY(util)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(audio)
ADD_SUBDIRECTORY(network)
ADD_SUBDIRECTORY(orxonox)
