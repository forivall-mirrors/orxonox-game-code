ORXONOX_ADD_EXECUTABLE(
    core_test
    EXCLUDE_FROM_ALL
    NO_INSTALL
  LINK_LIBRARIES
    core
    gmock_orxonox
  SOURCE_FILES
    ${GMOCK_MAIN}
    class/IdentifiableTest.cc
    class/IdentifierTest.cc
    class/OrxonoxClassTest.cc
    class/OrxonoxInterfaceTest.cc
    class/SubclassIdentifierTest.cc
    class/SuperTest.cc
)
ADD_DEPENDENCIES(test core_test)

ADD_TEST(core_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core_test --gtest_output=xml)
