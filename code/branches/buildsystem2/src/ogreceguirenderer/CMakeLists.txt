SET(OCR_FILES
  OgreCEGUIRenderer.h
  OgreCEGUIResourceProvider.h
  OgreCEGUITexture.h

  OgreCEGUIRenderer.cpp
  OgreCEGUIResourceProvider.cpp
  OgreCEGUITexture.cpp
)
GENERATE_SOURCE_GROUPS(${OCR_FILES})

ADD_LIBRARY(ogreceguirenderer_orxonox SHARED ${OCR_FILES})
SET_TARGET_PROPERTIES(ogreceguirenderer_orxonox PROPERTIES DEFINE_SYMBOL "OGRE_GUIRENDERER_EXPORTS")
TARGET_LINK_LIBRARIES(ogreceguirenderer_orxonox
  ${OGRE_LIBRARY}
  ${CEGUI_LIBRARY}
)
IF (NOT WIN32)
  INSTALL(TARGETS ogreceguirenderer_orxonox LIBRARY DESTINATION lib)
ENDIF (NOT WIN32)
