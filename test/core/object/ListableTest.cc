#include <gtest/gtest.h>
#include "core/object/Listable.h"
#include "core/CoreIncludes.h"
#include "core/module/ModuleInstance.h"

namespace orxonox
{
    namespace
    {
        class ListableClassTest : public Listable
        {
            public:
            ListableClassTest() { RegisterObject(ListableClassTest); }
        };

        class ListableSubclassTest : public ListableClassTest
        {
            public:
                ListableSubclassTest() { RegisterObject(ListableSubclassTest); }
        };

        RegisterClassNoArgs(ListableClassTest);
        RegisterClassNoArgs(ListableSubclassTest);

        template <class T>
        bool objectListContains(T* element, Context* context = Context::getRootContext())
        {
            for (typename ObjectList<T>::iterator it = static_cast<ObjectListElement<T>*>(context->getObjectList<T>()->begin());
                    it != static_cast<ObjectListElement<T>*>(context->getObjectList<T>()->end()); ++it)
                if (*it == element)
                    return true;
            return false;
        }

        // Fixture
        class ListableTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    Context::setRootContext(new Context(NULL));
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(0);
                }

                virtual void TearDown()
                {
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(0);
                    Context::setRootContext(NULL);
                }
        };
    }

    TEST_F(ListableTest, CanCreate)
    {
        ListableClassTest* test = new ListableClassTest();
        ASSERT_TRUE(test != NULL);
        delete test;
    }

    TEST_F(ListableTest, AddsToObjectList)
    {
        ListableClassTest test;
        EXPECT_EQ(1u, ObjectList<ListableClassTest>::size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test));
    }

    TEST_F(ListableTest, AddsToAllObjectLists)
    {
        ListableSubclassTest test;
        EXPECT_EQ(1u, ObjectList<ListableClassTest>::size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test));
        EXPECT_EQ(1u, ObjectList<ListableSubclassTest>::size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test));
    }

    TEST_F(ListableTest, RemovesFromObjectList)
    {
        EXPECT_EQ(0u, ObjectList<ListableClassTest>::size());
        {
            ListableClassTest test;
            EXPECT_EQ(1u, ObjectList<ListableClassTest>::size());
            EXPECT_TRUE(objectListContains<ListableClassTest>(&test));
        }
        EXPECT_EQ(0u, ObjectList<ListableClassTest>::size());
    }

    TEST_F(ListableTest, RemovesFromAllObjectLists)
    {
        EXPECT_EQ(0u, ObjectList<ListableClassTest>::size());
        EXPECT_EQ(0u, ObjectList<ListableSubclassTest>::size());
        {
            ListableSubclassTest test;
            EXPECT_EQ(1u, ObjectList<ListableClassTest>::size());
            EXPECT_TRUE(objectListContains<ListableClassTest>(&test));
            EXPECT_EQ(1u, ObjectList<ListableSubclassTest>::size());
            EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test));
        }
        EXPECT_EQ(0u, ObjectList<ListableClassTest>::size());
        EXPECT_EQ(0u, ObjectList<ListableSubclassTest>::size());
    }

    TEST_F(ListableTest, CanAddObjectToContext)
    {
        Context context(NULL);
        ListableSubclassTest test;

        test.setContext(&context);

        // object is in new context
        EXPECT_EQ(1u, context.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &context));
        EXPECT_EQ(1u, context.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &context));
    }

    TEST_F(ListableTest, CanAddObjectToSubContext)
    {
        Context baseContext(NULL);
        Context subContext(&baseContext);
        ListableSubclassTest test;

        test.setContext(&subContext);

        // object is in both contexts
        EXPECT_EQ(1u, baseContext.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &baseContext));
        EXPECT_EQ(1u, baseContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &baseContext));

        EXPECT_EQ(1u, subContext.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &subContext));
        EXPECT_EQ(1u, subContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext));
    }

    TEST_F(ListableTest, CanChangeContext)
    {
        Context* rootContext = Context::getRootContext();
        Context newContext(NULL);
        ListableSubclassTest test;

        // object is in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is NOT in new context
        EXPECT_EQ(0u, newContext.getObjectList<ListableClassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableClassTest>(&test, &newContext));
        EXPECT_EQ(0u, newContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &newContext));

        test.setContext(&newContext);

        // object was removed from root context
        EXPECT_EQ(0u, rootContext->getObjectList<ListableClassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableClassTest>(&test, rootContext));
        EXPECT_EQ(0u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is now in new context
        EXPECT_EQ(1u, newContext.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &newContext));
        EXPECT_EQ(1u, newContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &newContext));
    }

    TEST_F(ListableTest, CanChangeToSubContext)
    {
        Context* rootContext = Context::getRootContext();
        Context subContext(rootContext);
        ListableSubclassTest test;

        // object is in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is NOT in sub context
        EXPECT_EQ(0u, subContext.getObjectList<ListableClassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableClassTest>(&test, &subContext));
        EXPECT_EQ(0u, subContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &subContext));

        test.setContext(&subContext);

        // object is still in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is now also in sub context
        EXPECT_EQ(1u, subContext.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &subContext));
        EXPECT_EQ(1u, subContext.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext));
    }

    TEST_F(ListableTest, CanChangeFromSubcontextToSubContext)
    {
        Context* rootContext = Context::getRootContext();
        Context subContext1(rootContext);
        Context subContext2(rootContext);
        ListableSubclassTest test;

        test.setContext(&subContext1);

        // object is in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object is also in sub context 1
        EXPECT_EQ(1u, subContext1.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &subContext1));
        EXPECT_EQ(1u, subContext1.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext1));

        // object is NOT in sub context 2
        EXPECT_EQ(0u, subContext2.getObjectList<ListableClassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableClassTest>(&test, &subContext2));
        EXPECT_EQ(0u, subContext2.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &subContext2));

        test.setContext(&subContext2);

        // object is still in root context
        EXPECT_EQ(1u, rootContext->getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, rootContext));
        EXPECT_EQ(1u, rootContext->getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, rootContext));

        // object was removed from sub context 1
        EXPECT_EQ(0u, subContext1.getObjectList<ListableClassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableClassTest>(&test, &subContext1));
        EXPECT_EQ(0u, subContext1.getObjectList<ListableSubclassTest>()->size());
        EXPECT_FALSE(objectListContains<ListableSubclassTest>(&test, &subContext1));

        // object is now in sub context 2
        EXPECT_EQ(1u, subContext2.getObjectList<ListableClassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableClassTest>(&test, &subContext2));
        EXPECT_EQ(1u, subContext2.getObjectList<ListableSubclassTest>()->size());
        EXPECT_TRUE(objectListContains<ListableSubclassTest>(&test, &subContext2));
    }
}
