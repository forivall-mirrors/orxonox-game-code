#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "ClassHierarchy.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    class BaseObject : virtual public OrxonoxClass
    {
        public:
            BaseObject();
            ~BaseObject();

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

//            Identifier* identifier_;

//        protected:
//            IdentifierList* parents_; // INTERN! Don't touch this!

    };
}

#endif
