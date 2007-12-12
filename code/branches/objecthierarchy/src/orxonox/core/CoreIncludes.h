/**
    @file CoreIncludes.h
    @brief Definition of macros and typedefs.

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
#include "Debug.h"

#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"
#include "OgreQuaternion.h"
#include "OgreMatrix3.h"


// Some typedefs
namespace orxonox
{
    typedef Ogre::Vector2 Vector2;
    typedef Ogre::Vector3 Vector3;
    typedef Ogre::ColourValue ColourValue;
    typedef Ogre::Radian Radian;
    typedef Ogre::Real Real;
    typedef Ogre::Quaternion Quaternion;
    typedef Ogre::Matrix3 Matrix3;
}


/**
    @brief Intern macro, containing the common parts of RegisterObject and RegisterRootObject.
    @param ClassName The name of the class
    @param bRootClass True if the class is directly derived from OrxonoxClass
*/
#define InternRegisterObject(ClassName, bRootClass) \
    this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, bRootClass)); \
    if (Identifier::isCreatingHierarchy() && this->getParents()) \
        this->getParents()->add(this->getIdentifier()); \
    ClassIdentifier<ClassName>::addObject(this)

/**
    @brief Intern macro, containing the specific part of RegisterRootObject.
    @param ClassName The name of the class
*/
#define InternRegisterRootObject(ClassName) \
    if (Identifier::isCreatingHierarchy() && !this->getParents()) \
        this->setParents(new IdentifierList()); \
    InternRegisterObject(ClassName, true)

/**
    @brief RegisterObject - with and without debug output.
    @param ClassName The name of the class
*/
#define RegisterObject(ClassName) \
    COUT(4) << "*** Register Object: " << #ClassName << "\n"; \
    InternRegisterObject(ClassName, false)

/**
    @brief RegisterRootObject - with and without debug output.
    @param ClassName The name of the class
*/
#define RegisterRootObject(ClassName) \
    COUT(4) << "*** Register Root-Object: " << #ClassName << "\n"; \
    InternRegisterRootObject(ClassName)

/**
    @brief Returns the Identifier of the given class.
    @param ClassName The name of the class
*/
#define Class(ClassName) \
    ClassIdentifier<ClassName>::getIdentifier()

/**
    @brief Creates the entry in the Factory.
    @param ClassName The name of the class
*/
#define CreateFactory(ClassName) \
    bool bCreated##ClassName##Factory = ClassFactory<ClassName>::create(#ClassName)

/**
    @brief Returns the Identifier with either a given name or a given network ID through the factory.
    @param StringOrInt The name or the network ID of the class
*/
#define ID(StringOrInt) \
    Factory::getIdentifier(StringOrInt)

/**
    @brief Assigns the value, defined in the config-file, to the variable (or the default-value, if there is no entry in the file).
    @param varname The name of the variable
    @param defvalue The default-value of the variable
*/
#define SetConfigValue(varname, defvalue) \
    ConfigValueContainer* container##varname = this->getIdentifier()->getConfigValueContainer(#varname); \
    if (!container##varname) \
    { \
        container##varname = new ConfigValueContainer(this->getIdentifier()->getName(), #varname, defvalue); \
        this->getIdentifier()->setConfigValueContainer(#varname, container##varname); \
    } \
    varname = container##varname->getValue(varname)
