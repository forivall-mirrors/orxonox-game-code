#ifndef _BaseObject_H__
#define _BaseObject_H__

#include "ClassHierarchy.h"

namespace orxonox
{
    class BaseObject
    {
        public:
            BaseObject();
            ~BaseObject();

            inline bool isA(Identifier* identifier)
                { this->identifier_->isA(identifier); }
            inline bool isDirectA(Identifier* identifier)
                { this->identifier_->isDirectA(identifier); }
            inline bool isChildOf(Identifier* identifier)
                { this->identifier_->isChildOf(identifier); }
            inline bool isDirectChildOf(Identifier* identifier)
                { this->identifier_->isDirectChildOf(identifier); }
            inline bool isParentOf(Identifier* identifier)
                { this->identifier_->isParentOf(identifier); }
            inline bool isDirectParentOf(Identifier* identifier)
                { this->identifier_->isDirectParentOf(identifier); }

            ClassIdentifier<class BaseObject>* identifier_;

        protected:
            IdentifierList* parents_; // INTERN! Don't touch this!

        private:

    };
}

#endif
