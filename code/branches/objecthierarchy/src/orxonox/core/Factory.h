#ifndef _Factory_H__
#define _Factory_H__

#include <map>
#include <string>

namespace orxonox
{
    class BaseObject;
    class Identifier;

    // ###############################
    // ###         Factory         ###
    // ###############################
    class Factory
    {
        public:
            static Identifier* getIdentifier(const std::string& name);
            static Identifier* getIdentifier(const unsigned int id);
            static void add(const std::string& name, Identifier* identifier);
            static void changeNetworkID(Identifier* identifier, const unsigned int oldID, const unsigned int newID);

        private:
            Factory() {}
            Factory(const Factory& factory) {}
            ~Factory() {}

            static Factory* pointer_s;
            std::map<std::string, Identifier*> identifierStringMap_;
            std::map<unsigned int, Identifier*> identifierNetworkIDMap_;
    };

    // ###############################
    // ###       BaseFactory       ###
    // ###############################
    class BaseFactory
    {
        public:
            virtual BaseObject* fabricate() = 0;
    };
}

#endif
