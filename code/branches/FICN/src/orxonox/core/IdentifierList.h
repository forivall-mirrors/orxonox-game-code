#ifndef _IdentifierList_H__
/*!
    @file IdentifierList.h
    @brief Definition of the IdentifierList class

    The IdentifierList is a single-linked list, containing Identifiers.
    The IdentifierList is used to store parents and childs of each Identifier.
*/

#define _IdentifierList_H__

#include <string>

namespace orxonox
{
    class Identifier; // Forward declaration

    //! The list-element of the IdentifierList
    class IdentifierListElement
    {
        public:
            IdentifierListElement(const Identifier* identifier);

            const Identifier* identifier_;      //!< The identifier
            IdentifierListElement* next_;       //!< The next element in the list
    };

    //! The IdentifierList contains Identifiers
    class IdentifierList
    {
        public:
            IdentifierList();
            ~IdentifierList();
            void add(const Identifier* identifier);
            void remove(const Identifier* identifier);
            bool isInList(const Identifier* identifier) const;
            std::string toString() const;

            IdentifierListElement* first_;      //!< The first element in the list
    };
}

#endif /* _IdentifierList_H__ */
