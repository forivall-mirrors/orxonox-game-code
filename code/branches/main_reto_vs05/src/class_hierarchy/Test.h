#ifndef _Test_H__
#define _Test_H__

#include "IdentifierIncludes.h"

namespace orxonox
{
    class Interface1 : virtual public OrxonoxClass
    {
        public:
            Interface1() { registerRootObject(Interface1); }
    };

    class Interface2 : virtual public OrxonoxClass
    {
        public:
            Interface2() { registerRootObject(Interface2); }
    };

    class A1 : public BaseObject
    {
        public:
            A1() { registerObject(A1); }
    };

    class A2 : public BaseObject
    {
        public:
            A2() { registerObject(A2); }
    };

    class A3: public BaseObject, public Interface1
    {
        public:
            A3() { registerObject(A3); }
    };

    class A1B1 : public A1
    {
        public:
            A1B1() { registerObject(A1B1); }
    };

    class A1B2 : public A1
    {
        public:
            A1B2() { registerObject(A1B2); }
    };

    class A2B1 : public A2
    {
        public:
            A2B1() { registerObject(A2B1); }
    };

    class A2B2 : public A2, Interface1
    {
        public:
            A2B2() { registerObject(A2B2); }
    };

    class A3B1 : public A3
    {
        public:
            A3B1() { registerObject(A3B1); }
    };

    class A3B2 : public A3, Interface2
    {
        public:
            A3B2() { registerObject(A3B2); }
    };

    class A1B1C1 : public A1B1
    {
        public:
            A1B1C1() { registerObject(A1B1C1); }
    };

    class A1B1C2 : public A1B1
    {
        public:
            A1B1C2() { registerObject(A1B1C2); }
    };

    class A1B2C1 : public A1B2
    {
        public:
            A1B2C1() { registerObject(A1B2C1); }
    };

    class A2B1C1 : public A2B1, Interface2
    {
        public:
            A2B1C1() { registerObject(A2B1C1); }
    };

    class A2B2C1 : public A2B2
    {
        public:
            A2B2C1() { registerObject(A2B2C1); }
    };

    class A3B1C1 : public A3B1
    {
        public:
            A3B1C1() { registerObject(A3B1C1); }
    };

    class A3B1C2 : public A3B1, Interface2
    {
        public:
            A3B1C2() { registerObject(A3B1C2); }
    };

    class A3B2C1 : public A3B2
    {
        public:
            A3B2C1() { registerObject(A3B2C1); }
    };

    class A3B2C2 : public A3B2
    {
        public:
            A3B2C2() { registerObject(A3B2C2); }
    };
}

#endif
