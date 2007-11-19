#include "Factory.h"
#include "Identifier.h"

namespace orxonox
{
    ClassFactory* ClassFactory::pointer_ = NULL;

    OrxonoxClass* ClassFactory::fabricate(const std::string& name)
    {
        if (!pointer_)
            pointer_ = new ClassFactory;

        return pointer_->identifierMap_[name]->fabricate();
    }

    void ClassFactory::add(const std::string& name, Identifier* identifier)
    {
        if (!pointer_)
            pointer_ = new ClassFactory;

        pointer_->identifierMap_[name] = identifier;
    }
}
