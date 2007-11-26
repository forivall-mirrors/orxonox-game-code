#ifndef _Test1_H__
#define _Test1_H__

#include "BaseObject.h"
#include "test3.h"

namespace orxonox
{
    class Test1 : public BaseObject
    {
        public:
            Test1();
            virtual ~Test1();

            bool usefullClass1isA(Identifier* identifier);
            bool usefullClass2isA(Identifier* identifier);
            bool usefullClass3isA(Identifier* identifier);
            void setUsefullClass1(Identifier* identifier);
            void setUsefullClass2(Identifier* identifier);
            void setUsefullClassOfTypeTest3(Identifier* identifier);

        private:
            Identifier* usefullClass1_;
            Identifier* usefullClass2_;
            SubclassIdentifier<Test3> usefullClass3_;

    };
}

#endif
