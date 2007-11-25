#include "Identifier.h"
#include "Factory.h"
#include "ClassFactory.h"
//#include "IdentifierList.h"
//#include "ObjectList.h"
#include "Iterator.h"
#include "OrxonoxClass.h"


#define InternRegisterObject(ClassName, bRootClass) \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, bRootClass)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    ClassIdentifier<ClassName>::addObject(this)

#define InternRegisterRootObject(ClassName) \
    if (Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->setParents(new IdentifierList()); \
    InternRegisterObject(ClassName, true)

#if HIERARCHY_VERBOSE
#define RegisterObject(ClassName) \
    std::cout << "*** Register Object: " << #ClassName << "\n"; \
    InternRegisterObject(ClassName, false)
#else
#define RegisterObject(ClassName) \
    InternRegisterObject(ClassName, false)
#endif

#if HIERARCHY_VERBOSE
#define RegisterRootObject(ClassName) \
    std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
    InternRegisterRootObject(ClassName)
#else
#define RegisterRootObject(ClassName) \
    InternRegisterRootObject(ClassName)
#endif

#define Class(ClassName) \
    ClassIdentifier<ClassName>::getIdentifier()

#define CreateFactory(ClassName) \
    bool bCreated##ClassName##Factory = ClassFactory<ClassName>::create()

#define ID(Name) \
    Factory::getIdentifier(Name)
