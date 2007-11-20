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
//        this->directParents_ = new IdentifierList();
//        this->allParents_ = new IdentifierList();
//        this->directChildren_ = new IdentifierList();
//        this->allChildren_ = new IdentifierList();
//        this->objects_ = new ObjectList();
    }

    Identifier::~Identifier()
    {
//        delete this->directParents_;
//        delete this->allParents_;
//        delete this->directChildren_;
//        delete this->allChildren_;
//        delete this->objects_;
        delete &this->name_;
    }

    void Identifier::initialize(IdentifierList* parents)
    {
        std::cout << "*** Initialize " << this->name_ << "-Singleton.\n";
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
                    temp1->identifier_->directChildren_.add(this);
                }

                this->allParents_.add(temp1->identifier_);
                temp1->identifier_->allChildren_.add(this);

                temp1 = temp1->next_;
            }
        }
    }

    void Identifier::addObject(OrxonoxClass* object)
    {
        std::cout << "*** Added object to " << this->name_ << "-list.\n";
        this->objects_.add(object);
    }

    void Identifier::removeObject(OrxonoxClass* object)
    {
        bool bIterateForwards = !Identifier::isCreatingHierarchy();

        if (bIterateForwards)
            std::cout << "*** Removed object from " << this->name_ << "-list, iterating forwards.\n";
        else
            std::cout << "*** Removed object from " << this->name_ << "-list, iterating backwards.\n";

        this->objects_.remove(object, bIterateForwards);

        IdentifierListElement* temp = this->directParents_.first_;
        while (temp)
        {
            temp->identifier_->removeObject(object);
            temp = temp->next_;
        }
    }

    bool Identifier::isA(Identifier* identifier)
    {
        return (identifier == this || this->allParents_.isInList(identifier));
    }

    bool Identifier::isDirectlyA(Identifier* identifier)
    {
        return (identifier == this);
    }

    bool Identifier::isChildOf(Identifier* identifier)
    {
        return this->allParents_.isInList(identifier);
    }

    bool Identifier::isDirectChildOf(Identifier* identifier)
    {
        return this->directParents_.isInList(identifier);
    }

    bool Identifier::isParentOf(Identifier* identifier)
    {
        return this->allChildren_.isInList(identifier);
    }

    bool Identifier::isDirectParentOf(Identifier* identifier)
    {
        return this->directChildren_.isInList(identifier);
    }
}
