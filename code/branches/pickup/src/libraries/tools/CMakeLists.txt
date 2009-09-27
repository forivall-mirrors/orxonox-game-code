ADD_SOURCE_FILES(TOOLS_SRC_FILES
  BillboardSet.cc
  DynamicLines.cc
  DynamicRenderable.cc
  Mesh.cc
  ParticleInterface.cc
  ResourceCollection.cc
  ResourceLocation.cc
  Shader.cc
  TextureGenerator.cc
  Timer.cc
)
ADD_SUBDIRECTORY(interfaces)

ORXONOX_ADD_LIBRARY(tools
  FIND_HEADER_FILES
  PCH_FILE
    ToolsPrecompiledHeaders.h
  DEFINE_SYMBOL
    "TOOLS_SHARED_BUILD"
  LINK_LIBRARIES
    core
  SOURCE_FILES ${TOOLS_SRC_FILES}
)
