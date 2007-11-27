#include "IdentifierList.h"
#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###     IdentifierList      ###
    // ###############################
    IdentifierList::IdentifierList()
    {
        this->first_ = 0;
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

    std::string IdentifierList::toString()
    {
        IdentifierListElement* temp = this->first_;
        std::string output = "";

        while (temp)
        {
            output += temp->identifier_->getName();
            output += " ";

            temp = temp->next_;
        }

        return output;
    }


    // ###############################
    // ###  IdentifierListElement  ###
    // ###############################
    IdentifierListElement::IdentifierListElement(Identifier* identifier)
    {
        this->identifier_ = identifier;
        this->next_ = 0;
        this->bDirect_ = true;
    }

    IdentifierListElement::~IdentifierListElement()
    {
    }
}
