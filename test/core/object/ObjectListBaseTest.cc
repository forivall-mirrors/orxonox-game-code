#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/object/ObjectListBase.h"
#include "core/class/Identifier.h"

namespace orxonox
{
    namespace
    {
        class ObjectListElementRemovalListenerMock : public ObjectListElementRemovalListener
        {
            public:
                MOCK_METHOD1(removedElement, void(ObjectListBaseElement*));
        };
    }

    TEST(ObjectListBaseTest, EmptyListHasSizeZero)
    {
        ObjectListBase list;
        EXPECT_EQ(0u, list.size());
    }

    TEST(ObjectListBaseTest, CanAddElement)
    {
        ObjectListBase list;
        ObjectListBaseElement* element = new ObjectListElement<Listable>(NULL);
        list.addElement(element);
        EXPECT_EQ(1u, list.size());
    }

    TEST(ObjectListBaseTest, CanRemoveElement)
    {
        ObjectListBase list;
        ObjectListBaseElement* element = new ObjectListElement<Listable>(NULL);
        list.addElement(element);
        EXPECT_EQ(1u, list.size());
        list.removeElement(element);
        EXPECT_EQ(0u, list.size());
        delete element;
    }

    TEST(ObjectListBaseTest, CanAddObject)
    {
        ObjectListBase list;
        list.addElement(new ObjectListElement<Listable>(NULL));
        EXPECT_EQ(1u, list.size());
    }

    TEST(ObjectListBaseTest, HasCorrectSize)
    {
        ObjectListBase list;
        EXPECT_EQ(0u, list.size());
        {
            ObjectListElement<Listable> element1(NULL);
            list.addElement(&element1);
            EXPECT_EQ(1u, list.size());
            {
                ObjectListElement<Listable> element1(NULL);
                list.addElement(&element1);
                EXPECT_EQ(2u, list.size());
                {
                    ObjectListElement<Listable> element1(NULL);
                    list.addElement(&element1);
                    EXPECT_EQ(3u, list.size());
                }
                EXPECT_EQ(2u, list.size());
            }
            EXPECT_EQ(1u, list.size());
        }
        EXPECT_EQ(0u, list.size());
    }

    TEST(ObjectListBaseTest, CallsListenerOnRemoval)
    {
        ObjectListBase list;
        ObjectListElementRemovalListenerMock listener;
        list.registerRemovalListener(&listener);
        ObjectListBaseElement* element = new ObjectListElement<Listable>(NULL);
        list.addElement(element);
        EXPECT_CALL(listener, removedElement(element));
        list.removeElement(element);
        delete element;
    }
}
