ENABLE_TESTING()

ADD_CUSTOM_TARGET(test COMMAND ${CMAKE_CTEST_COMMAND} "$(ARGS)" COMMENT "Running tests")

CONFIGURE_FILE("CTestCustom.cmake" "CTestCustom.cmake" @ONLY)

INCLUDE_DIRECTORIES(
  ../src/external
  ../src/libraries
  ../src/orxonox
  ../src/modules
  ../src/external/gtest/include
  ../src/external/gmock/include
  ${CMAKE_BINARY_DIR}/src
  ${Boost_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIR}
  ${POCO_INCLUDE_DIR}
)

ADD_SUBDIRECTORY(util)
