#ifndef _Test1_H__
#define _Test1_H__

#include "core/BaseObject.h"
#include "Tickable.h"
#include "test3.h"
#include "util/tinyxml/tinyxml.h"

namespace orxonox
{
    class Test1 : public BaseObject, public Tickable
    {
        public:
            Test1();
            virtual ~Test1();

            virtual void tick(float dt);

            bool usefullClass1isA(Identifier* identifier);
            bool usefullClass2isA(Identifier* identifier);
            bool usefullClass3isA(Identifier* identifier);
            void setUsefullClass1(Identifier* identifier);
            void setUsefullClass2(Identifier* identifier);
            void setUsefullClassOfTypeTest3(Identifier* identifier);

            void loadParams(TiXmlElement* xmlElem);

        private:
            Identifier* usefullClass1_;
            Identifier* usefullClass2_;
            SubclassIdentifier<Test3> usefullClass3_;

    };
}

#endif
