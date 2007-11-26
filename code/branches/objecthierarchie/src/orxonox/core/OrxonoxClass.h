#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include "Identifier.h"
#include "IdentifierList.h"
#include "ObjectList.h"
#include "MetaObjectList.h"

namespace orxonox
{
    class OrxonoxClass
    {
        public:
            OrxonoxClass();
            virtual ~OrxonoxClass();
            inline Identifier* getIdentifier() const { return this->identifier_; }
            inline void setIdentifier(Identifier* identifier) { this->identifier_ = identifier; }
            inline IdentifierList* getParents() const { return this->parents_; }
            inline void setParents(IdentifierList* parents) { this->parents_ = parents; }
            inline MetaObjectList* getMetaList() { return this->metaList_; }

        private:
            Identifier* identifier_;
            IdentifierList* parents_;
            MetaObjectList* metaList_;
    };
}

#endif
