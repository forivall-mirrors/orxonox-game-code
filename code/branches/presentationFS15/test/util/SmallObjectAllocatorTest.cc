#include <gtest/gtest.h>
#include "util/SmallObjectAllocator.h"

namespace orxonox
{
    //////////////////////////////////////////
    // Allocate objects with primitive type //
    //////////////////////////////////////////

    class SmallObjectAllocatorTestInt : public testing::Test
    {
        public:
            SmallObjectAllocatorTestInt() : allocator_(sizeof(size_t), 10)
            {
                SmallObjectAllocatorTestInt::count++;
            }

        protected:
            size_t* create(size_t value)
            {
                void* chunk = this->allocator_.alloc();
                size_t* pointer = new (chunk) size_t;
                *pointer = value;

                return pointer;
            }

            void destroy(size_t* pointer)
            {
                this->allocator_.free(pointer);
            }

            SmallObjectAllocator allocator_;
            static size_t count;
    };

    size_t SmallObjectAllocatorTestInt::count = 0;

    TEST_F(SmallObjectAllocatorTestInt, Default)
    {
        // Test if the fixture was created
        EXPECT_EQ(1u, SmallObjectAllocatorTestInt::count);
    }

    TEST_F(SmallObjectAllocatorTestInt, Create)
    {
        // create an integer
        size_t* pointer = this->create(5);
        ASSERT_NE((void*)0, pointer);
        EXPECT_EQ(5u, *pointer);
    }

    TEST_F(SmallObjectAllocatorTestInt, CreateAndDestroy)
    {
        // create an integer
        size_t* pointer = this->create(5);
        ASSERT_NE((void*)0, pointer);
        EXPECT_EQ(5u, *pointer);

        // destroy it again
        this->destroy(pointer);
    }

    TEST_F(SmallObjectAllocatorTestInt, Create100)
    {
        std::vector<size_t*> pointers;

        // create 100 integers. this is greater than the amount of chunks per block, hence the allocator has to allocate multiple blocks
        static const size_t LIMIT = 100;
        for (size_t i = 0; i < LIMIT; ++i)
        {
            size_t* pointer = this->create(i);
            EXPECT_EQ(i, *pointer);
            pointers.push_back(pointer);
        }

        EXPECT_EQ(LIMIT, pointers.size());

        // check if all integers are still valid
        for (size_t i = 0; i < LIMIT; ++i)
        {
            EXPECT_EQ(i, *pointers[i]);
        }
    }

    TEST_F(SmallObjectAllocatorTestInt, Create100Destroy50)
    {
        std::vector<size_t*> pointers;

        // create 100 integers
        static const size_t LIMIT = 100;
        for (size_t i = 0; i < LIMIT; ++i)
        {
            size_t* pointer = this->create(i);
            EXPECT_EQ(i, *pointer);
            pointers.push_back(pointer);
        }

        EXPECT_EQ(LIMIT, pointers.size());

        // destroy the first 50
        for (size_t i = 0; i < LIMIT / 2; ++i)
        {
            this->destroy(pointers[i]);
        }

        // check if the remaining integers are still valid
        for (size_t i = LIMIT / 2; i < LIMIT; ++i)
        {
            EXPECT_EQ(i, *pointers[i]);
        }
    }

    TEST_F(SmallObjectAllocatorTestInt, Create100Destroy50Create100)
    {
        std::vector<size_t*> pointers;

        // create 100 integers
        static const size_t LIMIT = 100;
        for (size_t i = 0; i < LIMIT; ++i)
        {
            size_t* pointer = this->create(i);
            EXPECT_EQ(i, *pointer);
            pointers.push_back(pointer);
        }

        EXPECT_EQ(LIMIT, pointers.size());

        // destroy the first 50
        for (size_t i = 0; i < LIMIT / 2; ++i)
        {
            this->destroy(pointers[i]);
        }

        // create 100 more integers
        for (size_t i = LIMIT; i < LIMIT * 2; ++i)
        {
            size_t* pointer = this->create(i);
            EXPECT_EQ(i, *pointer);
            pointers.push_back(pointer);
        }

        EXPECT_EQ(LIMIT * 2, pointers.size());

        // check if the remaining and the new integers are valid
        for (size_t i = LIMIT / 2; i < LIMIT * 2; ++i)
        {
            EXPECT_EQ(i, *pointers[i]);
        }
    }


    /////////////////////////////////////////////////////
    // Allocate objects which are instances of a class //
    /////////////////////////////////////////////////////

    class SmallObjectAllocatorTestObject : public testing::Test
    {
        protected:
            class SmallObject
            {
                public:
                    static size_t total_s;

                    SmallObject(size_t value)
                    {
                        this->value_ = value;
                        SmallObject::total_s += this->value_;
                    }

                    ~SmallObject()
                    {
                        SmallObject::total_s -= this->value_;
                    }

                    size_t getValue() const
                    {
                        return this->value_;
                    }

                private:
                    size_t value_;
            };

        public:
            SmallObjectAllocatorTestObject() : allocator_(sizeof(SmallObject), 10)
            {
                SmallObjectAllocatorTestObject::count++;
                SmallObject::total_s = 0;
            }

        protected:
            SmallObject* create(size_t value)
            {
                // allocate memory
                void* chunk = this->allocator_.alloc();
                // execute placement new operator
                SmallObject* pointer = new (chunk) SmallObject(value);

                return pointer;
            }

            void destroy(SmallObject* pointer)
            {
                // call destructor
                pointer->~SmallObject();
                // free memory
                this->allocator_.free(pointer);
            }

            SmallObjectAllocator allocator_;
            static size_t count;
    };

    size_t SmallObjectAllocatorTestObject::count = 0;
    size_t SmallObjectAllocatorTestObject::SmallObject::total_s = 0;

    TEST_F(SmallObjectAllocatorTestObject, Default)
    {
        // Test if the fixture was created
        EXPECT_EQ(1u, SmallObjectAllocatorTestObject::count);
    }

    TEST_F(SmallObjectAllocatorTestObject, Create)
    {
        EXPECT_EQ(0u, SmallObject::total_s);

        // create an object
        SmallObject* pointer = this->create(5);
        ASSERT_NE((void*)0, pointer);
        EXPECT_EQ(5u, pointer->getValue());
        EXPECT_EQ(5u, SmallObject::total_s);
    }

    TEST_F(SmallObjectAllocatorTestObject, CreateAndDestroy)
    {
        EXPECT_EQ(0u, SmallObject::total_s);

        // create an object
        SmallObject* pointer = this->create(5);
        ASSERT_NE((void*)0, pointer);
        EXPECT_EQ(5u, pointer->getValue());
        EXPECT_EQ(5u, SmallObject::total_s);

        // destroy it again
        this->destroy(pointer);
        EXPECT_EQ(0u, SmallObject::total_s);
    }

    TEST_F(SmallObjectAllocatorTestObject, CreateAndDestroy100)
    {
        std::vector<SmallObject*> pointers;

        // create 100 objects. this is greater than the amount of chunks per block, hence the allocator has to allocate multiple blocks
        static const size_t LIMIT = 100;
        size_t total = 0;
        for (size_t i = 0; i < LIMIT; ++i)
        {
            SmallObject* pointer = this->create(i);
            EXPECT_EQ(i, pointer->getValue());
            pointers.push_back(pointer);
            total += i;
        }

        EXPECT_EQ(LIMIT, pointers.size());
        EXPECT_EQ(total, SmallObject::total_s);

        // check if all objects are still valid
        for (size_t i = 0; i < LIMIT; ++i)
        {
            EXPECT_EQ(i, pointers[i]->getValue());
        }

        // destroy all objects
        for (size_t i = 0; i < LIMIT; ++i)
        {
            this->destroy(pointers[i]);
        }

        EXPECT_EQ(0u, SmallObject::total_s);
    }
}
