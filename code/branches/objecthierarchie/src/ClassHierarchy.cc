#include "ClassHierarchy.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
//    Identifier* Identifier::pointer_ = NULL;
/*
    Identifier* Identifier::registerClass(IdentifierList* parents)
    {
        if (!pointer_)
        {
            pointer_ = new Identifier();
            pointer_->initialize(parents);
        }

        return pointer_;
    }
*/
    Identifier::Identifier()
    {
        this->bCreatedOneObject_ = false;
        this->directParents_ = new IdentifierList();
        this->allParents_ = new IdentifierList();
        this->directChildren_ = new IdentifierList();
        this->allChildren_ = new IdentifierList();
        this->objects_ = new ObjectList();
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
                temp2 = temp1->identifier_->directParents_->first_;
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
                    this->directParents_->add(temp1->identifier_);
                    temp1->identifier_->directChildren_->add(this);
                }

                this->allParents_->add(temp1->identifier_);
                temp1->identifier_->allChildren_->add(this);

                temp1 = temp1->next_;
            }
        }
    }

    void Identifier::addObject(OrxonoxClass* object)
    {
        std::cout << "*** Added " << this->name_ << " to list.\n";
        this->objects_->add(object);
    }

    void Identifier::removeObject(OrxonoxClass* object)
    {
        std::cout << "*** Removed " << this->name_ << " from list.\n";
        this->objects_->remove(object);
    }

    bool Identifier::isA(Identifier* identifier)
    {
        return (identifier == this || this->allParents_->isInList(identifier));
    }

    bool Identifier::isDirectA(Identifier* identifier)
    {
        return (identifier == this);
    }

    bool Identifier::isChildOf(Identifier* identifier)
    {
        return this->allParents_->isInList(identifier);
    }

    bool Identifier::isDirectChildOf(Identifier* identifier)
    {
        return this->directParents_->isInList(identifier);
    }

    bool Identifier::isParentOf(Identifier* identifier)
    {
        return this->allChildren_->isInList(identifier);
    }

    bool Identifier::isDirectParentOf(Identifier* identifier)
    {
        return this->directChildren_->isInList(identifier);
    }


    // ###############################
    // ###     IdentifierList      ###
    // ###############################
    IdentifierList::IdentifierList()
    {
        this->first_ = NULL;
    }

    IdentifierList::~IdentifierList()
    {
        IdentifierListElement* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    void IdentifierList::add(Identifier* identifier)
    {
        IdentifierListElement* temp = this->first_;
        this->first_ = new IdentifierListElement(identifier);
        this->first_->next_ = temp;
    }

    void IdentifierList::remove(Identifier* identifier)
    {
        if (!identifier)
            return;

        if (this->first_->identifier_ == identifier)
        {
            IdentifierListElement* temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;

            return;
        }

        IdentifierListElement* temp = this->first_;
        while (temp->next_)
        {
            if (temp->next_->identifier_ == identifier)
            {
                IdentifierListElement* temp2 = temp->next_->next_;
                delete temp->next_;
                temp->next_ = temp2;

                return;
            }

            temp = temp->next_;
        }
    }

    bool IdentifierList::isInList(Identifier* identifier)
    {
        IdentifierListElement* temp = this->first_;
        while (temp)
        {
            if (temp->identifier_ == identifier)
                return true;

            temp = temp->next_;
        }

        return false;
    }


    // ###############################
    // ###  IdentifierListElement  ###
    // ###############################
    IdentifierListElement::IdentifierListElement(Identifier* identifier)
    {
        this->identifier_ = identifier;
        this->next_ = NULL;
        this->bDirect_ = true;
    }


    // ###############################
    // ###       ObjectList        ###
    // ###############################
    ObjectList::ObjectList()
    {
        this->first_ = NULL;
    }

    ObjectList::~ObjectList()
    {
        ObjectListElement* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    void ObjectList::add(OrxonoxClass* object)
    {
        ObjectListElement* temp = this->first_;
        this->first_ = new ObjectListElement(object);
        this->first_->next_ = temp;
    }

    void ObjectList::remove(OrxonoxClass* object)
    {
        if (!object)
            return;

        if (this->first_->object_ == object)
        {
            ObjectListElement* temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;

            return;
        }

        ObjectListElement* temp = this->first_;
        while (temp->next_)
        {
            if (temp->next_->object_ == object)
            {
                ObjectListElement* temp2 = temp->next_->next_;
                delete temp->next_;
                temp->next_ = temp2;

                return;
            }

            temp = temp->next_;
        }
    }


    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    ObjectListElement::ObjectListElement(OrxonoxClass* object)
    {
        this->object_ = object;
        this->next_ = NULL;
    }


    // ###############################
    // ###     ClassHierarchy      ###
    // ###############################
    ClassHierarchy* ClassHierarchy::pointer_ = NULL;

    ClassHierarchy* ClassHierarchy::getSingleton()
    {
        if (!pointer_)
            pointer_ = new ClassHierarchy();

        return pointer_;
    }

    ClassHierarchy::ClassHierarchy()
    {
        this->bCreatingHierarchy_ = false;
    }
}
