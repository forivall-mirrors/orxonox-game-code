#ifndef _Test3_H__
#define _Test3_H__

#include "BaseObject.h"

namespace orxonox
{
    class Test1;
    class Test2;

    class Test3 : public BaseObject
    {
        public:
            Test3();
            virtual ~Test3();

            bool usefullClassesIsATest(Test1* test1);
            bool usefullClassesIsATest(Test2* test2);
    };
}

#endif
