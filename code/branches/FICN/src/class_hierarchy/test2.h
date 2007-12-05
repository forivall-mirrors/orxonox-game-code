#ifndef _Test2_H__
#define _Test2_H__

#include "BaseObject.h"

namespace orxonox
{
    class Test3;

    class Test2 : public BaseObject
    {
        public:
            Test2();
            virtual ~Test2();

            bool usefullClass1isA(Identifier* identifier);
            bool usefullClass2isA(Identifier* identifier);
            bool usefullClass3isA(Identifier* identifier);
            void setUsefullClass1(Identifier* identifier);
            void setUsefullClass2(Identifier* identifier);
            void setUsefullClassOfTypeTest3(Identifier* identifier);

        private:
            Identifier* usefullClass1_;
            Identifier* usefullClass2_;
            BaseIdentifier<Test3> usefullClass3_;

    };
}

#endif
