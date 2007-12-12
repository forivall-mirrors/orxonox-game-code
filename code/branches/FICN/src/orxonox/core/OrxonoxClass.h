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

#include "../../tinyxml/tinyxml.h"


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

            inline bool isA(const Identifier* identifier)
                { return this->getIdentifier()->isA(identifier); }
            inline bool isDirectlyA(const Identifier* identifier)
                { return this->getIdentifier()->isDirectlyA(identifier); }
            inline bool isChildOf(const Identifier* identifier)
                { return this->getIdentifier()->isChildOf(identifier); }
            inline bool isParentOf(const Identifier* identifier)
                { return this->getIdentifier()->isParentOf(identifier); }

            inline bool isA(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isA(identifier->getIdentifier()); }
            inline bool isDirectlyA(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectlyA(identifier->getIdentifier()); }
            inline bool isChildOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
            inline bool isParentOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isParentOf(identifier->getIdentifier()); }

            inline bool isA(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isA(identifier.getIdentifier()); }
            inline bool isDirectlyA(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectlyA(identifier.getIdentifier()); }
            inline bool isChildOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
            inline bool isParentOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isParentOf(identifier.getIdentifier()); }

            inline bool isA(const OrxonoxClass* object)
                { return this->getIdentifier()->isA(object->getIdentifier()); }
            inline bool isDirectlyA(const OrxonoxClass* object)
                { return this->getIdentifier()->isDirectlyA(object->getIdentifier()); }
            inline bool isChildOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isChildOf(object->getIdentifier()); }
            inline bool isParentOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isParentOf(object->getIdentifier()); }


            inline void setName(const std::string& name) { this->name_ = name; }
            inline const std::string& getName() const { return this->name_; }

            inline void setActive(bool bActive) { this->bActive_ = bActive; }
            inline const bool isActive() const { return this->bActive_; }

            inline void setVisible(bool bVisible) { this->bVisible_ = bVisible; }
            inline const bool isVisible() const { return this->bVisible_; }


        private:
            Identifier* identifier_;        //!< The Identifier of the object
            IdentifierList* parents_;       //!< List of all parents of the object
            MetaObjectList metaList_;       //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in

            std::string name_;
            bool bActive_;
            bool bVisible_;
    };
}

#endif
