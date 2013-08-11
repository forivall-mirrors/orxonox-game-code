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
    class/IdentifierClassHierarchyTest.cc
    class/IdentifierSimpleClassHierarchyTest.cc
    class/IdentifierExternalClassHierarchyTest.cc
    class/OrxonoxClassTest.cc
    class/OrxonoxInterfaceTest.cc
    class/SubclassIdentifierTest.cc
    class/SuperTest.cc
    command/CommandTest.cc
    object/ClassFactoryTest.cc
    object/ContextTest.cc
    object/DestroyableTest.cc
    object/IteratorTest.cc
    object/ListableTest.cc
    object/ObjectListBaseTest.cc
    object/ObjectListIteratorTest.cc
    object/SmartPtrTest.cc
    object/WeakPtrTest.cc
)
ADD_DEPENDENCIES(test core_test)

ADD_TEST(core_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core_test --gtest_output=xml)
