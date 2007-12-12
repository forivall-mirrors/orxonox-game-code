/*!
    @file Identifier.cc
    @brief Implementation of the Identifier class.
*/

#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    int Identifier::hierarchyCreatingCounter_s = 0; // Set the static member variable hierarchyCreatingCounter_s to zero
    unsigned int Identifier::classIDcounter_s = 0; // Set the static member variable classIDcounter_s to zero

    /**
        @brief Constructor: No factory, no object created, new ObjectList and a unique networkID.
    */
    Identifier::Identifier()
    {
        this->bCreatedOneObject_ = false;
        this->factory_ = 0;

        this->children_ = new IdentifierList;
        this->classID_ = Identifier::classIDcounter_s++;
    }

    /**
        @brief Destructor: Deletes the name and the IdentifierList containing the children.
    */
    Identifier::~Identifier()
    {
        delete &this->name_;
        delete this->children_;
    }

    /**
        @brief Initializes the Identifier with an IdentifierList containing all parents of the class the Identifier belongs to.
        @param parents The IdentifierList containing all parents
    */
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
                temp1->identifier_->getChildren().add(this); // We're a child of our parents

                temp1 = temp1->next_;
            }
        }
    }

    /**
        @brief Creates an object of the type the Identifier belongs to.
        @return The new object
    */
    BaseObject* Identifier::fabricate()
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(); // We have to return a BaseObject, because we don't know the exact type.
        }
        else
        {
            // Abstract classes don't have a factory and therefore can't create new objects
            std::cout << "Error: Cannot create an object of type '" << this->name_ << "'. Class is abstract.\n";
            std::cout << "Aborting...";
            abort();
        }
    }

    /**
        @brief Sets the networkID to a new value and changes the entry in the Factory.
        @param id The new networkID
    */
    void Identifier::setNetworkID(unsigned int id)
    {
        Factory::changeNetworkID(this, this->classID_, id);
        this->classID_ = id;
    }

    /**
        @returns true, if the Identifier is at least of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || this->parents_.isInList(identifier));
    }

    /**
        @returns true, if the Identifier is exactly of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectlyA(const Identifier* identifier) const
    {
        return (identifier == this);
    }

    /**
        @returns true, if the assigned identifier is a child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isChildOf(const Identifier* identifier) const
    {
        return this->parents_.isInList(identifier);
    }

    /**
        @returns true, if the assigned identifier is a parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return this->children_->isInList(identifier);
    }
}
