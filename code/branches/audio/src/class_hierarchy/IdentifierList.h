#ifndef _IdentifierList_H__
#define _IdentifierList_H__

#include <string>

namespace orxonox
{
    class Identifier;

    class IdentifierListElement
    {
        public:
            IdentifierListElement(Identifier* identifier);
            ~IdentifierListElement();

            Identifier* identifier_;
            IdentifierListElement* next_;
            bool bDirect_;
    };

    class IdentifierList
    {
        public:
            IdentifierList();
            ~IdentifierList();
            void add(Identifier* identifier);
            void remove(Identifier* identifier);
            bool isInList(Identifier* identifier);
            std::string toString();

            IdentifierListElement* first_;
    };
}

#endif
