ENABLE_TESTING()

ADD_CUSTOM_TARGET(test COMMAND ${CMAKE_CTEST_COMMAND} "$(ARGS)" COMMENT "Running tests")

ADD_EXECUTABLE(sample_test EXCLUDE_FROM_ALL SampleTest.cc)
ADD_DEPENDENCIES(test sample_test)

ADD_TEST(sample_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sample_test)


IF(Boost_UNIT_TEST_FRAMEWORK_FOUND)
  INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
  )
  ADD_EXECUTABLE(boost_test EXCLUDE_FROM_ALL BoostTest.cc)
  TARGET_LINK_LIBRARIES(boost_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  ADD_DEPENDENCIES(test boost_test)

  ADD_TEST(boost_test_sample ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/boost_test)
ENDIF()

INCLUDE_DIRECTORIES(
  ../src/external/gtest/include
  ../src/external/gmock/include
)

ADD_EXECUTABLE(
    gtest_sample
    EXCLUDE_FROM_ALL
    ${GTEST_MAIN}
    gtest_samples/sample1.cc
    gtest_samples/sample1_unittest.cc
    gtest_samples/sample2.cc
    gtest_samples/sample2_unittest.cc
    gtest_samples/sample3_unittest.cc
    gtest_samples/sample4.cc
    gtest_samples/sample4_unittest.cc
)
TARGET_LINK_LIBRARIES(gtest_sample gtest_orxonox)
ADD_DEPENDENCIES(test gtest_sample)

ADD_TEST(gtest_sample ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gtest_sample)

#-----------------

CONFIGURE_FILE("CTestCustom.cmake" "CTestCustom.cmake" @ONLY)

INCLUDE_DIRECTORIES(
  ../src/libraries
  ../src/orxonox
  ../src/modules
  ${CMAKE_BINARY_DIR}/src
  ${OGRE_INCLUDE_DIR}
  ${POCO_INCLUDE_DIR}
)

ADD_SUBDIRECTORY(util)
