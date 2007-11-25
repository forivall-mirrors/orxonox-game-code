#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    int Identifier::hierarchyCreatingCounter_s = 0;

    Identifier::Identifier()
    {
        this->bCreatedOneObject_ = false;

        this->children_ = new IdentifierList;
    }

    Identifier::~Identifier()
    {
        delete &this->name_;

        delete this->children_;
    }

    void Identifier::initialize(const IdentifierList* parents)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Initialize " << this->name_ << "-Singleton.\n";
#endif
        if (parents)
        {
            this->bCreatedOneObject_ = true;

            IdentifierListElement* temp1 = parents->first_;
            while (temp1)
            {
                this->parents_.add(temp1->identifier_);
                temp1->identifier_->getChildren().add(this);

                temp1 = temp1->next_;
            }
        }
    }

    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || this->parents_.isInList(identifier));
    }

    bool Identifier::isDirectlyA(const Identifier* identifier) const
    {
        return (identifier == this);
    }

    bool Identifier::isChildOf(const Identifier* identifier) const
    {
        return this->parents_.isInList(identifier);
    }

    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return this->children_->isInList(identifier);
    }
}
