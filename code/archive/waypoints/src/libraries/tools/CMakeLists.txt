SET_SOURCE_FILES(TOOLS_SRC_FILES

BUILD_UNIT ResourceBuildUnit.cc
  ResourceCollection.cc
  ResourceLocation.cc
END_BUILD_UNIT

  TextureGenerator.cc
  Timer.cc

BUILD_UNIT OgreBuildUnit.cc
  BillboardSet.cc
  DynamicLines.cc
  DynamicRenderable.cc
  Mesh.cc
  ParticleInterface.cc
  Shader.cc
END_BUILD_UNIT
)

#ADD_SUBDIRECTORY(bsp)
ADD_SUBDIRECTORY(interfaces)

ORXONOX_ADD_LIBRARY(tools
  FIND_HEADER_FILES
  LINK_LIBRARIES
    ${OGRE_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY} # Filesystem dependency
    tinyxml_orxonox
    util
    core
  SOURCE_FILES
    ${TOOLS_SRC_FILES}
)
