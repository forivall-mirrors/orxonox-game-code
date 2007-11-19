#include "Identifier.h"
#include "Factory.h"
#include "IdentifierList.h"
#include "ObjectList.h"
#include "OrxonoxClass.h"


#define registerRootObject(ClassName) \
    std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
    if (Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->setParents(new IdentifierList()); \
    if (this->getIdentifier()) \
        this->getIdentifier()->removeObject(this); \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, true, false)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    this->getIdentifier()->addObject(this)

#define registerAbstractRootObject(ClassName) \
    std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
    if (Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->setParents(new IdentifierList()); \
    if (this->getIdentifier()) \
        this->getIdentifier()->removeObject(this); \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, true, true)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    this->getIdentifier()->addObject(this)

#define registerObject(ClassName) \
    std::cout << "*** Register Object: " << #ClassName << "\n"; \
    this->getIdentifier()->removeObject(this); \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, false, false)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    this->getIdentifier()->addObject(this)

#define registerAbstractObject(ClassName) \
    std::cout << "*** Register Object: " << #ClassName << "\n"; \
    this->getIdentifier()->removeObject(this); \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, false, true)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    this->getIdentifier()->addObject(this)

#define unregisterObject() \
    this->getIdentifier()->removeObject(this)

#define Class(ClassName) \
    ClassIdentifier<ClassName>::getIdentifier()

#define CreateFactory(ClassName) \
    Identifier* global_##ClassName##_Identifier = ClassIdentifier<ClassName>::getIdentifier()

#define Factory(Name) \
    ClassFactory::fabricate(Name)
