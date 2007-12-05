#include "ClassHierarchy.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    // ###############################
    // ###     ClassHierarchy      ###
    // ###############################
    ClassHierarchy* ClassHierarchy::pointer_ = NULL;

    ClassHierarchy* ClassHierarchy::getSingleton()
    {
        if (!pointer_)
            pointer_ = new ClassHierarchy();

        return pointer_;
    }

    ClassHierarchy::ClassHierarchy()
    {
        this->hierarchyCreatingCounter_ = 0;
    }

    ClassHierarchy::~ClassHierarchy()
    {
      this->pointer_ = NULL;
    }
}
