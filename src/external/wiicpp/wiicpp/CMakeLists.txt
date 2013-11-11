INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../wiic/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../log/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(WIICPP_SOURCES wiicpp.cpp ../log/logger.cpp ../log/dataset.cpp ../log/training.cpp ../log/gyrosample.cpp ../log/accsample.cpp)
#ADD_LIBRARY(wiicpp SHARED ${WIICPP_SOURCES})
#TARGET_LINK_LIBRARIES(wiicpp wiic)

#INSTALL(TARGETS wiicpp DESTINATION /usr/local/lib)
INSTALL(FILES wiicpp.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES ../log/training.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES ../log/dataset.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES ../log/sample.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES ../log/accsample.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES ../log/gyrosample.h DESTINATION /usr/local/include/wiic/)
INSTALL(FILES ../log/logger.h DESTINATION /usr/local/include/wiic/)

SET_SOURCE_FILES(WIICPP_FILES
  wiicpp.h
  wiicpp.cpp
)

ORXONOX_ADD_LIBRARY(wiicpp_orxonox
  ORXONOX_EXTERNAL
  NO_DLL_INTERFACE
  VERSION
    1.1
  LINK_LIBRARIES
    wiic_orxonox
  SOURCE_FILES
    ${WIICPP_SOURCES}
	
)

