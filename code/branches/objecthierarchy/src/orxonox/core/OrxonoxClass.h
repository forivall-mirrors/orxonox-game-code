/*!
    @file OrxonoxClass.h
    @brief Definition of the OrxonoxClass Class.

    All objects and interfaces of the game-logic are derived from OrxonoxClass.
    It stores the Identifier and the MetaObjectList and has all needed functions to create the class-hierarchy.
*/

#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include "Identifier.h"
#include "IdentifierList.h"
#include "ObjectList.h"
#include "MetaObjectList.h"

namespace orxonox
{
    //! The class all objects and interfaces of the game-logic are derived from.
    /**
        BaseObject and Interaces are derived with 'virtual public OrxonoxClass' from OrxonoxClass.
        OrxonoxClass is needed to create the class-hierarchy at startup and to store the Identifier and the MetaObjectList.
    */
    class OrxonoxClass
    {
        public:
            OrxonoxClass();
            virtual ~OrxonoxClass();

            /** @returns the Identifier of the object */
            inline Identifier* getIdentifier() const { return this->identifier_; }

            /** @brief Sets the Identifier of the object. Used by the RegisterObject-macro. */
            inline void setIdentifier(Identifier* identifier) { this->identifier_ = identifier; }

            /** @returns the list of all parents of the object */
            inline IdentifierList* getParents() const { return this->parents_; }

            /** @brief Sets the Parents of the object. Used by the RegisterObject-macro. */
            inline void setParents(IdentifierList* parents) { this->parents_ = parents; }

            /** @returns the MetaObjectList of the object, containing a link to all ObjectLists and ObjectListElements the object is registered in. */
            inline MetaObjectList& getMetaList() { return this->metaList_; }

        private:
            Identifier* identifier_;        //!< The Identifier of the object
            IdentifierList* parents_;       //!< List of all parents of the object
            MetaObjectList metaList_;       //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in
    };
}

#endif
