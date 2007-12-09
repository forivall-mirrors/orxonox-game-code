/*!
    @file IdentifierList.cc
    @brief Implementation of the IdentifierList class.
*/

#include "IdentifierList.h"
#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###     IdentifierList      ###
    // ###############################
    /**
        @brief Constructor: Sets first_ to zero.
    */
    IdentifierList::IdentifierList()
    {
        this->first_ = 0;
    }

    /**
        @brief Destructor: Deletes all elements in the list, but NOT THE IDENTIFIERS.
    */
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

    /**
        @brief Adds an Identifier to the list.
        @param identifier The Identifier to add
    */
    void IdentifierList::add(const Identifier* identifier)
    {
        IdentifierListElement* temp = this->first_;
        this->first_ = new IdentifierListElement(identifier);
        this->first_->next_ = temp;
    }

    /**
        @brief Removes an Identifier from the list.
        @param identifier The Identifier to remove
    */
    void IdentifierList::remove(const Identifier* identifier)
    {
        if (!identifier)
            return;

        // Check if we have to delete the first element
        if (this->first_->identifier_ == identifier)
        {
            IdentifierListElement* temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;

            return;
        }

        // Iterate through the list
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

    /**
        @brief Checks if a given Identifier is in the list and returns true if yes.
        @param identifier The Identifier to check
        @return True if the Identifier is in the list
    */
    bool IdentifierList::isInList(const Identifier* identifier) const
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

    /**
        @returns a string, containing a list of the names of all Identifiers in the list.
    */
    std::string IdentifierList::toString() const
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
    /**
        @brief Constructor: Creates the list-element with a given identifier.
        @param identifier The Identifier to store
    */
    IdentifierListElement::IdentifierListElement(const Identifier* identifier)
    {
        this->identifier_ = identifier;
        this->next_ = 0;
    }
}
