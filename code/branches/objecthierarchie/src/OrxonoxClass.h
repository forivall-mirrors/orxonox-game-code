#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

namespace orxonox
{
    class Identifier;
    class IdentifierList;

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
