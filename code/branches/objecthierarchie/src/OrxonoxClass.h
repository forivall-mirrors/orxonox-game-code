#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include "Identifier.h"
#include "IdentifierList.h"

namespace orxonox
{
    class OrxonoxClass
    {
        public:
            OrxonoxClass();
            virtual ~OrxonoxClass();
            Identifier* getIdentifier() const { return this->identifier_; }
            void setIdentifier(Identifier* identifier) { this->identifier_ = identifier; }
            IdentifierList* getParents() const { return this->parents_; }
            void setParents(IdentifierList* parents) { this->parents_ = parents; }

        private:
            Identifier* identifier_;
            IdentifierList* parents_;
    };
}

#endif
