/**
    @file CoreIncludes.h
    @brief Definition of macros for the class-hierarchy and the factory.

    Every class needs the RegisterObject(class) macro in its constructor. If the class is an interface
    or the BaseObject itself, it needs the macro RegisterRootObject(class) instead.

    To allow the object being created through the factory, use the CreateFactory(class) macro outside
    the of the class implementation, so it gets executed before main().
*/

// All needed header-files
#include "Identifier.h"
#include "Factory.h"
#include "ClassFactory.h"
#include "Iterator.h"
#include "OrxonoxClass.h"
#include "ConfigValueContainer.h"

#include "OgreVector3.h"
#include "OgreColourValue.h"

namespace orxonox
{
    typedef Ogre::Vector3 Vector3;
    typedef Ogre::ColourValue ColourValue;
}

// Intern macro, containing the common parts of RegisterObject and RegisterRootObject
#define InternRegisterObject(ClassName, bRootClass) \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, bRootClass)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    ClassIdentifier<ClassName>::addObject(this)

// Intern macro, containing the specific part of RegisterRootObject
#define InternRegisterRootObject(ClassName) \
    if (Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->setParents(new IdentifierList()); \
    InternRegisterObject(ClassName, true)

// RegisterObject - with and without debug output
#if HIERARCHY_VERBOSE
#define RegisterObject(ClassName) \
    std::cout << "*** Register Object: " << #ClassName << "\n"; \
    InternRegisterObject(ClassName, false)
#else
#define RegisterObject(ClassName) \
    InternRegisterObject(ClassName, false)
#endif

// RegisterRootObject - with and without debug output
#if HIERARCHY_VERBOSE
#define RegisterRootObject(ClassName) \
    std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
    InternRegisterRootObject(ClassName)
#else
#define RegisterRootObject(ClassName) \
    InternRegisterRootObject(ClassName)
#endif

// Class(ClassName) returns the Identifier of the given class
#define Class(ClassName) \
    ClassIdentifier<ClassName>::getIdentifier()

// Creates the entry in the Factory
#define CreateFactory(ClassName) \
    bool bCreated##ClassName##Factory = ClassFactory<ClassName>::create()

// ID(StringOrInt) returns the Identifier with either a given name or a given NetworkID through the factory
#define ID(StringOrInt) \
    Factory::getIdentifier(StringOrInt)

// bla
#define SetConfigValue(varname, defvalue) \
    ConfigValueContainer* container##varname = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        container##varname = new ConfigValueContainer(this->getIdentifier()->getName(), #varname, defvalue); \
        this->getIdentifier()->setConfigValueContainer(#varname, container##varname); \
    } \
    this->varname = container##varname->getValue(varname)
