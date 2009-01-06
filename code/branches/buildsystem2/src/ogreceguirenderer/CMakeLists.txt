SET( SRC_FILES
  OgreCEGUIRenderer.cpp
  OgreCEGUIResourceProvider.cpp
  OgreCEGUITexture.cpp
)

ADD_LIBRARY(ogreceguirenderer_orxonox SHARED ${SRC_FILES})
INSTALL(TARGETS ogreceguirenderer_orxonox LIBRARY DESTINATION lib)
