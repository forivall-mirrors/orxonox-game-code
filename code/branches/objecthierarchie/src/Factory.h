#ifndef _Factory_H__
#define _Factory_H__

#include <map>
#include <string>

namespace orxonox
{
    class Identifier;
    class BaseObject;

    class ClassFactory
    {
        public:
            static BaseObject* fabricate(const std::string& name);
            static void add(const std::string& name, Identifier* identifier);

        private:
            ClassFactory() {}
            ClassFactory(const ClassFactory& factory) {}
            ~ClassFactory() {}

            static ClassFactory* pointer_s;
            std::map<std::string, Identifier*> identifierMap_;
    };
}

#endif
