#include "Factory.h"
#include "Identifier.h"
#include "BaseObject.h"

namespace orxonox
{
    ClassFactory* ClassFactory::pointer_s = NULL;

    BaseObject* ClassFactory::fabricate(const std::string& name)
    {
        if (!pointer_s)
            pointer_s = new ClassFactory;

        return pointer_s->identifierMap_[name]->fabricate();
    }

    void ClassFactory::add(const std::string& name, Identifier* identifier)
    {
        if (!pointer_s)
            pointer_s = new ClassFactory;

        pointer_s->identifierMap_[name] = identifier;
    }
}
