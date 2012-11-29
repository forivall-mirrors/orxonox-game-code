ORXONOX_ADD_EXECUTABLE(
    util_test
    EXCLUDE_FROM_ALL
	NO_INSTALL
  LINK_LIBRARIES
    util
	gmock_orxonox
  SOURCE_FILES
    ${GMOCK_MAIN}
    ConvertTest.cc
    MathTest.cc
    mboolTest.cc
    MultiTypeTest.cc
    ScopeTest.cc
    SharedPtrTest.cc
    SingletonTest.cc
    SmallObjectAllocatorTest.cc
    StringUtilsTest.cc
    SubStringTest.cc
    VA_NARGSTest.cc
)
ADD_DEPENDENCIES(test util_test)

ADD_TEST(util_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_test)
