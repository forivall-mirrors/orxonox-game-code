ADD_EXECUTABLE(
    util_test
    EXCLUDE_FROM_ALL
    ${GMOCK_MAIN}
    ConvertTest.cc
    MathTest.cc
    mboolTest.cc
    ScopeTest.cc
    SharedPtrTest.cc
    SingletonTest.cc
    SmallObjectAllocatorTest.cc
    StringUtilsTest.cc
    SubStringTest.cc
    VA_NARGSTest.cc
)
TARGET_LINK_LIBRARIES(util_test util gmock_orxonox)
ADD_DEPENDENCIES(test util_test)

ADD_TEST(util_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_test)

#-----------------

IF(MSVC)
  IF(CMAKE_CL_64)
    SET(MSVC_PLATFORM "x64")
  ELSE()
    SET(MSVC_PLATFORM "Win32")
  ENDIF()
  IF(MSVC10)
    CONFIGURE_FILE("../../src/orxonox-main.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/util_test.vcxproj.user")
  ELSE()
    STRING(REGEX REPLACE "^Visual Studio ([0-9][0-9]?).*$" "\\1"
           VISUAL_STUDIO_VERSION_SIMPLE "${CMAKE_GENERATOR}")
    CONFIGURE_FILE("../../src/orxonox-main.vcproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/util_test.vcproj.user")
  ENDIF()
ENDIF(MSVC)
