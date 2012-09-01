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
ADD_SUBDIRECTORY(interfaces)

ORXONOX_ADD_LIBRARY(tools
  FIND_HEADER_FILES
  DEFINE_SYMBOL
    "TOOLS_SHARED_BUILD"
  LINK_LIBRARIES
    core
    network
  SOURCE_FILES ${TOOLS_SRC_FILES}
)
