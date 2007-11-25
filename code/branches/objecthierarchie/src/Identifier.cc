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

        this->directChildren_ = new IdentifierList;
        this->allChildren_ = new IdentifierList;
    }

    Identifier::~Identifier()
    {
        delete &this->name_;

        delete this->directChildren_;
        delete this->allChildren_;
    }

    void Identifier::initialize(const IdentifierList* parents)
    {
#if HIERARCHY_VERBOSE
        std::cout << "*** Initialize " << this->name_ << "-Singleton.\n";
#endif
        if (parents)
        {
            this->bCreatedOneObject_ = true;

            IdentifierListElement* temp1;
            IdentifierListElement* temp2;
            IdentifierListElement* temp3;

            temp1 = parents->first_;
            while (temp1)
            {
                temp2 = temp1->identifier_->directParents_.first_;
                while (temp2)
                {
                    temp3 = parents->first_;
                    while(temp3)
                    {
                        if (temp3->identifier_ == temp2->identifier_)
                            temp3->bDirect_ = false;

                        temp3 = temp3->next_;
                    }

                    temp2 = temp2->next_;
                }
                temp1 = temp1->next_;
            }

            temp1 = parents->first_;
            while (temp1)
            {
                if (temp1->bDirect_)
                {
                    this->directParents_.add(temp1->identifier_);
                    temp1->identifier_->getDirectChildren().add(this);
                }

                this->allParents_.add(temp1->identifier_);
                temp1->identifier_->getAllChildren().add(this);

                temp1 = temp1->next_;
            }
        }
    }

    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || this->allParents_.isInList(identifier));
    }

    bool Identifier::isDirectlyA(const Identifier* identifier) const
    {
        return (identifier == this);
    }

    bool Identifier::isChildOf(const Identifier* identifier) const
    {
        return this->allParents_.isInList(identifier);
    }

    bool Identifier::isDirectChildOf(const Identifier* identifier) const
    {
        return this->directParents_.isInList(identifier);
    }

    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return this->allChildren_->isInList(identifier);
    }

    bool Identifier::isDirectParentOf(const Identifier* identifier) const
    {
        return this->directChildren_->isInList(identifier);
    }
}
