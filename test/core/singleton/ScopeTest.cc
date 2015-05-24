#include <gtest/gtest.h>
#include "core/singleton/ScopedSingletonWrapper.h"

namespace orxonox
{
    namespace
    {
        class TestSingletonRoot : public Singleton<TestSingletonRoot>
        {
            friend class Singleton<TestSingletonRoot>;
            static TestSingletonRoot* singletonPtr_s;
        };
        class TestSingletonGraphics : public Singleton<TestSingletonGraphics>
        {
            friend class Singleton<TestSingletonGraphics>;
            static TestSingletonGraphics* singletonPtr_s;
        };

        ManageScopedSingleton(TestSingletonRoot, ScopeID::Root, false);
        ManageScopedSingleton(TestSingletonGraphics, ScopeID::Graphics, false);
    }

    TEST(Scope, ScopesDoNotExist)
    {
        EXPECT_FALSE(Scope<ScopeID::Root>::isActive());
        EXPECT_FALSE(Scope<ScopeID::Graphics>::isActive());
    }

    TEST(Scope, SingletonsDoNotExist)
    {
        EXPECT_FALSE(TestSingletonRoot::exists());
        EXPECT_FALSE(TestSingletonGraphics::exists());
    }

    TEST(Scope, RootScope)
    {
        EXPECT_FALSE(Scope<ScopeID::Root>::isActive());
        {   // create root scope
            Scope<ScopeID::Root> scope;
            EXPECT_TRUE(Scope<ScopeID::Root>::isActive());
        }   // destroy root scope
        EXPECT_FALSE(Scope<ScopeID::Root>::isActive());
    }

    TEST(DISABLED_Scope, RootAndGraphicsScope)
    {
        EXPECT_FALSE(Scope<ScopeID::Graphics>::isActive());
        {   // create root scope
            Scope<ScopeID::Root> scope;
            EXPECT_FALSE(Scope<ScopeID::Graphics>::isActive());
            {   // create graphics scope
                Scope<ScopeID::Graphics> scope;
                EXPECT_TRUE(Scope<ScopeID::Graphics>::isActive());
            }   // destroy graphics scope
            EXPECT_FALSE(Scope<ScopeID::Graphics>::isActive());
        }   // destroy root scope
        EXPECT_FALSE(Scope<ScopeID::Graphics>::isActive());
    }

    TEST(Scope, RootSingleton)
    {
        EXPECT_FALSE(TestSingletonRoot::exists());
        {   // create root scope
            Scope<ScopeID::Root> scope;
            EXPECT_TRUE(TestSingletonRoot::exists());
        }   // destroy root scope
        EXPECT_FALSE(TestSingletonRoot::exists());
    }

    TEST(DISABLED_Scope, RootAndGraphicsSingleton)
    {
        EXPECT_FALSE(TestSingletonGraphics::exists());
        {   // create root scope
            Scope<ScopeID::Root> scope;
            EXPECT_FALSE(TestSingletonGraphics::exists());
            {   // create graphics scope
                Scope<ScopeID::Graphics> scope;
                EXPECT_TRUE(TestSingletonGraphics::exists());
            }   // destroy graphics scope
            EXPECT_FALSE(TestSingletonGraphics::exists());
        }   // destroy root scope
        EXPECT_FALSE(TestSingletonGraphics::exists());
    }
}
