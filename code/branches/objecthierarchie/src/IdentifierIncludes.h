#include "Identifier.h"
#include "Factory.h"
#include "IdentifierList.h"
#include "ObjectList.h"
#include "Iterator.h"
#include "OrxonoxClass.h"


#define internRegisterRootObject(ClassName, bAbstract) \
    if (Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->setParents(new IdentifierList()); \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, true, bAbstract)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    this->getIdentifier()->addObject(this)

#define registerRootObject(ClassName) \
    std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
    internRegisterRootObject(ClassName, false)

#define registerAbstractRootObject(ClassName) \
    std::cout << "*** Register abstract Root-Object: " << #ClassName << "\n"; \
    internRegisterRootObject(ClassName, true)

#define internRegisterObject(ClassName, bAbstract) \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, false, bAbstract)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    this->getIdentifier()->addObject(this)

#define registerObject(ClassName) \
    std::cout << "*** Register Object: " << #ClassName << "\n"; \
    internRegisterObject(ClassName, false)

#define registerAbstractObject(ClassName) \
    std::cout << "*** Register abstract Object: " << #ClassName << "\n"; \
    internRegisterObject(ClassName, true)

#define unregisterObject() \
    this->getIdentifier()->removeObject(this)

#define Class(ClassName) \
    ClassIdentifier<ClassName>::getIdentifier()

#define CreateFactory(ClassName) \
    Identifier* global_##ClassName##_Identifier = ClassIdentifier<ClassName>::getIdentifier()

#define Factory(Name) \
    ClassFactory::fabricate(Name)
