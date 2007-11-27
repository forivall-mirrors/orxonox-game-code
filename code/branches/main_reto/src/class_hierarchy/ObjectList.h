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
    };

    class ObjectList
    {
        public:
            ObjectList();
            ~ObjectList();
            void add(OrxonoxClass* object);
            void remove(OrxonoxClass* object);

            ObjectListElement* first_;
    };
}

#endif
