#include <gtest/gtest.h>
#include "core/object/ClassFactory.h"
#include "core/BaseObject.h"
#include "core/object/Context.h"
#include "core/module/ModuleInstance.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    namespace
    {
        // Fixture
        class ClassFactoryTest : public ::testing::Test
        {
            public:
                virtual void SetUp()
                {
                    new IdentifierManager();
                    ModuleInstance::getCurrentModuleInstance()->loadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                    Context::setRootContext(new Context(NULL));
                }

                virtual void TearDown()
                {
                    Context::destroyRootContext();
                    ModuleInstance::getCurrentModuleInstance()->unloadAllStaticallyInitializedInstances(StaticInitialization::IDENTIFIER);
                    delete &IdentifierManager::getInstance();
                }
        };
    }

    TEST_F(ClassFactoryTest, CanFabricateObject)
    {
        Factory* factory = new ClassFactoryWithContext<BaseObject>();
        Identifiable* object = factory->fabricate(NULL);
        ASSERT_TRUE(object != NULL);
        BaseObject* baseObject = dynamic_cast<BaseObject*>(object);
        EXPECT_TRUE(baseObject != NULL);
        delete object;
        // don't delete factory - it remains in the identifier
    }
}
