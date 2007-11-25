#ifndef _IdentifierList_H__
#define _IdentifierList_H__

#include <string>

namespace orxonox
{
    class Identifier;

    class IdentifierListElement
    {
        public:
            IdentifierListElement(const Identifier* identifier);
            ~IdentifierListElement();

            const Identifier* identifier_;
            IdentifierListElement* next_;
            bool bDirect_;
    };

    class IdentifierList
    {
        public:
            IdentifierList();
            ~IdentifierList();
            void add(const Identifier* identifier);
            void remove(const Identifier* identifier);
            bool isInList(const Identifier* identifier) const;
            std::string toString() const;

            IdentifierListElement* first_;
    };
}

#endif
