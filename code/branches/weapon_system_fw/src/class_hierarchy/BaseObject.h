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
                { return this->getIdentifier()->isA(identifier); }
            inline bool isDirectlyA(Identifier* identifier)
                { return this->getIdentifier()->isDirectlyA(identifier); }
            inline bool isChildOf(Identifier* identifier)
                { return this->getIdentifier()->isChildOf(identifier); }
            inline bool isDirectChildOf(Identifier* identifier)
                { return this->getIdentifier()->isDirectChildOf(identifier); }
            inline bool isParentOf(Identifier* identifier)
                { return this->getIdentifier()->isParentOf(identifier); }
            inline bool isDirectParentOf(Identifier* identifier)
                { return this->getIdentifier()->isDirectParentOf(identifier); }

            inline bool isA(BaseIdentifier<class B>* identifier)
                { return this->getIdentifier()->isA(identifier->getIdentifier()); }
            inline bool isDirectlyA(BaseIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectlyA(identifier->getIdentifier()); }
            inline bool isChildOf(BaseIdentifier<class B>* identifier)
                { return this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
            inline bool isDirectChildOf(BaseIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectChildOf(identifier->getIdentifier()); }
            inline bool isParentOf(BaseIdentifier<class B>* identifier)
                { return this->getIdentifier()->isParentOf(identifier->getIdentifier()); }
            inline bool isDirectParentOf(BaseIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectParentOf(identifier->getIdentifier()); }

            inline bool isA(BaseIdentifier<class B> identifier)
                { return this->getIdentifier()->isA(identifier.getIdentifier()); }
            inline bool isDirectlyA(BaseIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectlyA(identifier.getIdentifier()); }
            inline bool isChildOf(BaseIdentifier<class B> identifier)
                { return this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
            inline bool isDirectChildOf(BaseIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectChildOf(identifier.getIdentifier()); }
            inline bool isParentOf(BaseIdentifier<class B> identifier)
                { return this->getIdentifier()->isParentOf(identifier.getIdentifier()); }
            inline bool isDirectParentOf(BaseIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectParentOf(identifier.getIdentifier()); }
/*
            inline bool isA(OrxonoxClass* object)
                { return this->getIdentifier()->isA(object->getIdentifier()); }
            inline bool isDirectlyA(OrxonoxClass* object)
                { return this->getIdentifier()->isDirectlyA(object->getIdentifier); }
            inline bool isChildOf(OrxonoxClass* object)
                { return this->getIdentifier()->isChildOf(object->getIdentifier); }
            inline bool isDirectChildOf(OrxonoxClass* object)
                { return this->getIdentifier()->isDirectChildOf(object->getIdentifier); }
            inline bool isParentOf(OrxonoxClass* object)
                { return this->getIdentifier()->isParentOf(object->getIdentifier); }
            inline bool isDirectParentOf(OrxonoxClass* object)
                { return this->getIdentifier()->isDirectParentOf(object->getIdentifier); }
*/

    };
}

#endif
