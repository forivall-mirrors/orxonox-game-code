SET_SOURCE_FILES(TOOLS_SRC_FILES
COMPILATION_BEGIN ResourceCompilation.cc
  ResourceCollection.cc
  ResourceLocation.cc
COMPILATION_END
  TextureGenerator.cc
  Timer.cc
COMPILATION_BEGIN OgreCompilation.cc
  BillboardSet.cc
  DynamicLines.cc
  DynamicRenderable.cc
  Mesh.cc
  ParticleInterface.cc
  Shader.cc
COMPILATION_END
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
