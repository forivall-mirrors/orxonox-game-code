SET( SRC_FILES
  OgreCEGUIRenderer.cpp
  OgreCEGUIResourceProvider.cpp
  OgreCEGUITexture.cpp
)

ADD_LIBRARY(ogreceguirenderer_orxonox SHARED ${SRC_FILES})

TARGET_LINK_LIBRARIES(ogreceguirenderer_orxonox
  ${OGRE_LIBRARY}
  ${CEGUI_LIBRARY}
)
IF (NOT WIN32)
  INSTALL(TARGETS ogreceguirenderer_orxonox LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
