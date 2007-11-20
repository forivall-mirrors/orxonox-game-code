#ifndef _ObjectList_H__
#define _ObjectList_H__

namespace orxonox
{
    class OrxonoxClass;

    class ObjectListElement
    {
        public:
            ObjectListElement(OrxonoxClass* object);
            ~ObjectListElement();

            OrxonoxClass* object_;
            ObjectListElement* next_;
            ObjectListElement* prev_;
    };

    class ObjectList
    {
        public:
            ObjectList();
            ~ObjectList();
            void add(OrxonoxClass* object);
            void remove(OrxonoxClass* object, bool bIterateForwards = true);

            ObjectListElement* first_;
            ObjectListElement* last_;
    };
}

#endif
