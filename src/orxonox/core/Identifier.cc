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
        this->factory_ = 0;

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
        this->bCreatedOneObject_ = true;

        if (parents)
        {
            IdentifierListElement* temp1 = parents->first_;
            while (temp1)
            {
                this->parents_.add(temp1->identifier_);
                temp1->identifier_->getChildren().add(this);

                temp1 = temp1->next_;
            }
        }
    }

    BaseObject* Identifier::fabricate()
    {
        if (this->factory_)
        {
            return this->factory_->fabricate();
        }
        else
        {
            std::cout << "Error: Cannot create an object of type '" << this->name_ << "'. Class is abstract.\n";
            std::cout << "Aborting...";
            abort();
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
