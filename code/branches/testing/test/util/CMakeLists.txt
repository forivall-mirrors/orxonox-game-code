ADD_EXECUTABLE(
    util_test
    EXCLUDE_FROM_ALL
    ${GMOCK_MAIN}
    MathTest.cc
)
TARGET_LINK_LIBRARIES(util_test util gmock_orxonox)
ADD_DEPENDENCIES(test util_test)

ADD_TEST(util_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_test)
