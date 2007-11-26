#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "IdentifierIncludes.h"

namespace orxonox
{
    class BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            virtual ~BaseObject();

            inline bool isA(const Identifier* identifier)
                { this->getIdentifier()->isA(identifier); }
            inline bool isDirectlyA(const Identifier* identifier)
                { this->getIdentifier()->isDirectlyA(identifier); }
            inline bool isChildOf(const Identifier* identifier)
                { this->getIdentifier()->isChildOf(identifier); }
            inline bool isParentOf(const Identifier* identifier)
                { this->getIdentifier()->isParentOf(identifier); }

            inline bool isA(const SubclassIdentifier<class B>* identifier)
                { this->getIdentifier()->isA(identifier->getIdentifier()); }
            inline bool isDirectlyA(const SubclassIdentifier<class B>* identifier)
                { this->getIdentifier()->isDirectlyA(identifier->getIdentifier()); }
            inline bool isChildOf(const SubclassIdentifier<class B>* identifier)
                { this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
            inline bool isParentOf(const SubclassIdentifier<class B>* identifier)
                { this->getIdentifier()->isParentOf(identifier->getIdentifier()); }

            inline bool isA(const SubclassIdentifier<class B> identifier)
                { this->getIdentifier()->isA(identifier.getIdentifier()); }
            inline bool isDirectlyA(const SubclassIdentifier<class B> identifier)
                { this->getIdentifier()->isDirectlyA(identifier.getIdentifier()); }
            inline bool isChildOf(const SubclassIdentifier<class B> identifier)
                { this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
            inline bool isParentOf(const SubclassIdentifier<class B> identifier)
                { this->getIdentifier()->isParentOf(identifier.getIdentifier()); }

            inline bool isA(const OrxonoxClass* object)
                { this->getIdentifier()->isA(object->getIdentifier()); }
            inline bool isDirectlyA(const OrxonoxClass* object)
                { this->getIdentifier()->isDirectlyA(object->getIdentifier()); }
            inline bool isChildOf(const OrxonoxClass* object)
                { this->getIdentifier()->isChildOf(object->getIdentifier()); }
            inline bool isParentOf(const OrxonoxClass* object)
                { this->getIdentifier()->isParentOf(object->getIdentifier()); }

            std::string name_; // test

    };
}

#endif
