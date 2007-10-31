#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "ClassHierarchy.h"

//namespace orxonox
//{
    class BaseObject
    {
        public:
            BaseObject();
            ~BaseObject();

            bool isA(ClassName* className);
            bool isChildOf(ClassName* className);
            bool isParentOf(ClassName* className);
            static bool getParentOf(ClassName* className);

//            static void operator new (size_t size);

            ClassName *className;

        private:

    };
//}

#endif
