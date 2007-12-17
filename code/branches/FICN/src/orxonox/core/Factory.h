/*!
    @file Factory.h
    @brief Definition of the Factory and the BaseFactory class.

    The Factory is a singleton, containing two maps to map either the name or the network ID
    of a class with the corresponding Identifier.

    Usage:
    ID(classname) or ID(networkID) returns the corresponding Identifier.


    BaseObject is the parent of ClassFactory which is defined in ClassFactory.h.
    It can't be defined in ClassFactory.h, because of circular dependencies.
*/

#ifndef _Factory_H__
#define _Factory_H__

#include <map>
#include <string>

namespace orxonox
{
    class BaseObject; // Forward declaration
    class Identifier; // Forward declaration

    // ###############################
    // ###         Factory         ###
    // ###############################
    //! The Factory is used to map the name or the network ID of a class with its Identifier.
    class Factory
    {
        public:
            static Identifier* getIdentifier(const std::string& name);
            static Identifier* getIdentifier(const unsigned int id);
            static void add(const std::string& name, Identifier* identifier);
            static void changeNetworkID(Identifier* identifier, const unsigned int oldID, const unsigned int newID);
            static void createClassHierarchy();
            static Factory* getFactoryPointer();// avoid overriding pointer_s in the static intialisation process

        private:
            Factory() {}                            // don't create
            Factory(const Factory& factory) {}      // don't copy
            ~Factory() {}                           // don't delete

            static Factory* pointer_s;
            std::map<std::string, Identifier*> identifierStringMap_;            //!< The map, mapping the name with the Identifier
            std::map<unsigned int, Identifier*> identifierNetworkIDMap_;        //!< The map, mapping the network ID with the Identifier
    };

    // ###############################
    // ###       BaseFactory       ###
    // ###############################
    //! Base-class of ClassFactory. Has to be defined separate because of circular dependencies.
    class BaseFactory
    {
        public:
            virtual BaseObject* fabricate() = 0;
            virtual ~BaseFactory() {};
    };
}

#endif
