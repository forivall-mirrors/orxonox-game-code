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

            inline bool isA(Identifier* identifier)
                { this->getIdentifier()->isA(identifier); }
            inline bool isDirectlyA(Identifier* identifier)
                { this->getIdentifier()->isDirectlyA(identifier); }
            inline bool isChildOf(Identifier* identifier)
                { this->getIdentifier()->isChildOf(identifier); }
            inline bool isDirectChildOf(Identifier* identifier)
                { this->getIdentifier()->isDirectChildOf(identifier); }
            inline bool isParentOf(Identifier* identifier)
                { this->getIdentifier()->isParentOf(identifier); }
            inline bool isDirectParentOf(Identifier* identifier)
                { this->getIdentifier()->isDirectParentOf(identifier); }

            inline bool isA(BaseIdentifier<class B>* identifier)
                { this->getIdentifier()->isA(identifier->getIdentifier()); }
            inline bool isDirectlyA(BaseIdentifier<class B>* identifier)
                { this->getIdentifier()->isDirectlyA(identifier->getIdentifier()); }
            inline bool isChildOf(BaseIdentifier<class B>* identifier)
                { this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
            inline bool isDirectChildOf(BaseIdentifier<class B>* identifier)
                { this->getIdentifier()->isDirectChildOf(identifier->getIdentifier()); }
            inline bool isParentOf(BaseIdentifier<class B>* identifier)
                { this->getIdentifier()->isParentOf(identifier->getIdentifier()); }
            inline bool isDirectParentOf(BaseIdentifier<class B>* identifier)
                { this->getIdentifier()->isDirectParentOf(identifier->getIdentifier()); }

            inline bool isA(BaseIdentifier<class B> identifier)
                { this->getIdentifier()->isA(identifier.getIdentifier()); }
            inline bool isDirectlyA(BaseIdentifier<class B> identifier)
                { this->getIdentifier()->isDirectlyA(identifier.getIdentifier()); }
            inline bool isChildOf(BaseIdentifier<class B> identifier)
                { this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
            inline bool isDirectChildOf(BaseIdentifier<class B> identifier)
                { this->getIdentifier()->isDirectChildOf(identifier.getIdentifier()); }
            inline bool isParentOf(BaseIdentifier<class B> identifier)
                { this->getIdentifier()->isParentOf(identifier.getIdentifier()); }
            inline bool isDirectParentOf(BaseIdentifier<class B> identifier)
                { this->getIdentifier()->isDirectParentOf(identifier.getIdentifier()); }
/*
            inline bool isA(OrxonoxClass* object)
                { this->getIdentifier()->isA(object->getIdentifier()); }
            inline bool isDirectlyA(OrxonoxClass* object)
                { this->getIdentifier()->isDirectlyA(object->getIdentifier); }
            inline bool isChildOf(OrxonoxClass* object)
                { this->getIdentifier()->isChildOf(object->getIdentifier); }
            inline bool isDirectChildOf(OrxonoxClass* object)
                { this->getIdentifier()->isDirectChildOf(object->getIdentifier); }
            inline bool isParentOf(OrxonoxClass* object)
                { this->getIdentifier()->isParentOf(object->getIdentifier); }
            inline bool isDirectParentOf(OrxonoxClass* object)
                { this->getIdentifier()->isDirectParentOf(object->getIdentifier); }
*/

    };
}

#endif
