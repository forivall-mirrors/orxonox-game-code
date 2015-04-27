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
    OutputTest.cc
    SharedPtrTest.cc
    SingletonTest.cc
    SmallObjectAllocatorTest.cc
    StringUtilsTest.cc
    SubStringTest.cc
    VA_NARGSTest.cc

    output/BaseWriterTest.cc
    output/ConsoleWriterTest.cc
    output/LogWriterTest.cc
    output/MemoryWriterTest.cc
    output/OutputDefinitionsTest.cc
    output/OutputListenerTest.cc
    output/OutputManagerTest.cc
    output/OutputStreamTest.cc
    output/SubcontextOutputListenerTest.cc
)
ADD_DEPENDENCIES(all_tests util_test)

ADD_TEST(util_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_test --gtest_output=xml)
