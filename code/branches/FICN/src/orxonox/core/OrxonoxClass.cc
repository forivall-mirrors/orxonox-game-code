/*!
    @file OrxonoxClass.cc
    @brief Implementation of the OrxonoxClass Class.
*/

#include "OrxonoxClass.h"

namespace orxonox
{
    /** @brief Constructor: Sets the default values. */
    OrxonoxClass::OrxonoxClass()
    {
        this->identifier_ = 0;
        this->parents_ = 0;

        this->bActive_ = true;
        this->bVisible_ = true;
    }

    /** @brief Destructor: Deletes, if existing, the list of the parents. */
    OrxonoxClass::~OrxonoxClass()
    {
        // parents_ exists only if isCreatingHierarchy() of the associated Identifier returned true while creating the class
        if (this->parents_)
            delete this->parents_;
    }
}
