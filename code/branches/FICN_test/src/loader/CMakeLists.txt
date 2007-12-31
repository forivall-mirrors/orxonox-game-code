SET( LOADER_SRC_FILES
  LevelLoader.cc
)

ADD_LIBRARY(${Orxonox_BINARY_DIR}/loader SHARED ${LOADER_SRC_FILES})
SET_TARGET_PROPERTIES( ${Orxonox_BINARY_DIR}/loader PROPERTIES LINK_FLAGS "--no-undefined" )

TARGET_LINK_LIBRARIES( ${Orxonox_BINARY_DIR}/loader
  ${Orxonox_BINARY_DIR}/util
)