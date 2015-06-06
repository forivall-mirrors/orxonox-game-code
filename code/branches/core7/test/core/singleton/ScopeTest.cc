#include <gtest/gtest.h>
#include "core/singleton/ScopedSingletonIncludes.h"
#include "core/module/ModuleInstance.h"
#include "core/CoreIncludes.h"

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

        ManageScopedSingleton(TestSingletonRoot, ScopeID::ROOT, false);
        ManageScopedSingleton(TestSingletonGraphics, ScopeID::GRAPHICS, false);

        // Fixture
        class ScopeTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(StaticInitialization::SCOPED_SINGLETON_WRAPPER);
                }

                virtual void TearDown()
                {
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(StaticInitialization::SCOPED_SINGLETON_WRAPPER);
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                }
        };
    }

    TEST_F(ScopeTest, ScopesDoNotExist)
    {
        EXPECT_FALSE(Scope<ScopeID::ROOT>::isActive());
        EXPECT_FALSE(Scope<ScopeID::GRAPHICS>::isActive());
    }

    TEST_F(ScopeTest, SingletonsDoNotExist)
    {
        EXPECT_FALSE(TestSingletonRoot::exists());
        EXPECT_FALSE(TestSingletonGraphics::exists());
    }

    TEST_F(ScopeTest, RootScope)
    {
        EXPECT_FALSE(Scope<ScopeID::ROOT>::isActive());
        {   // create root scope
            Scope<ScopeID::ROOT> scope;
            EXPECT_TRUE(Scope<ScopeID::ROOT>::isActive());
        }   // destroy root scope
        EXPECT_FALSE(Scope<ScopeID::ROOT>::isActive());
    }

    TEST_F(ScopeTest, DISABLED_RootAndGraphicsScope)
    {
        EXPECT_FALSE(Scope<ScopeID::GRAPHICS>::isActive());
        {   // create root scope
            Scope<ScopeID::ROOT> scope;
            EXPECT_FALSE(Scope<ScopeID::GRAPHICS>::isActive());
            {   // create graphics scope
                Scope<ScopeID::GRAPHICS> scope;
                EXPECT_TRUE(Scope<ScopeID::GRAPHICS>::isActive());
            }   // destroy graphics scope
            EXPECT_FALSE(Scope<ScopeID::GRAPHICS>::isActive());
        }   // destroy root scope
        EXPECT_FALSE(Scope<ScopeID::GRAPHICS>::isActive());
    }

    TEST_F(ScopeTest, RootSingleton)
    {
        EXPECT_FALSE(TestSingletonRoot::exists());
        {   // create root scope
            Scope<ScopeID::ROOT> scope;
            EXPECT_TRUE(TestSingletonRoot::exists());
        }   // destroy root scope
        EXPECT_FALSE(TestSingletonRoot::exists());
    }

    TEST_F(ScopeTest, DISABLED_RootAndGraphicsSingleton)
    {
        EXPECT_FALSE(TestSingletonGraphics::exists());
        {   // create root scope
            Scope<ScopeID::ROOT> scope;
            EXPECT_FALSE(TestSingletonGraphics::exists());
            {   // create graphics scope
                Scope<ScopeID::GRAPHICS> scope;
                EXPECT_TRUE(TestSingletonGraphics::exists());
            }   // destroy graphics scope
            EXPECT_FALSE(TestSingletonGraphics::exists());
        }   // destroy root scope
        EXPECT_FALSE(TestSingletonGraphics::exists());
    }
}
