#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include "IdentifierList.h"
#include "Identifier.h"

namespace orxonox
{
    class OrxonoxClass
    {
        public:
            OrxonoxClass();
            ~OrxonoxClass();
            Identifier* getIdentifier() { return this->identifier_; }
            void setIdentifier(Identifier* identifier) { this->identifier_ = identifier; }
            IdentifierList* getParents() { return this->parents_; }
            void setParents(IdentifierList* parents) { this->parents_ = parents; }

        private:
            Identifier* identifier_;
            IdentifierList* parents_;
    };
}

#endif
