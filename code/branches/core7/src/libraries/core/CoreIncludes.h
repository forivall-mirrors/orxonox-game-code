/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @defgroup Factory RegisterObject() and RegisterClass()
    @ingroup Object
*/

/**
    @file
    @ingroup Object Factory Class Identifier
    @brief Defines several very important macros used to register objects, register classes, and to work with identifiers.

    Every class needs the @c RegisterObject(class) macro in its constructor.

    To register @a class in the class-hierarchy, use the @c RegisterClass(class) macro outside of the class implementation,
    so it gets executed statically before @c main(). If you don't want @a class to be loadable, but still register it, call
    @c RegisterUnloadableClass(class).

    Abstract classes are registered with @c RegisterAbstractClass(class). For abstract classes, the inheritance must be
    defined manually with @c RegisterAbstractClass(class).inheritsFrom(Class(parent)). Multiple parent classes can be defined
    by chaining the above command.

    Example:
    @code
    // register MyClass
    RegisterClass(MyClass);

    // Constructor:
    MyClass::MyClass()
    {
        // Register the object in the Identifier of MyClass
        RegisterObject(MyClass);
    }
    @endcode

    This file also defines a number of other useful macros, like, for example, @c Class(class) which
    returns the @ref orxonox::Identifier "Identifier" of @a class, or @c ClassByString("class") which
    returns the Identifier of a class with name @a "class".

    Example:
    @code
    // Assigns the Identifier of MyClass
    Identifier* identifier = Class(MyClass);
    @endcode
    @code
    // Assigns the Identifier of a class named "MyClass"
    Identifier* identifier = ClassByString("MyClass");
    @endcode
*/

#ifndef _CoreIncludes_H__
#define _CoreIncludes_H__

#include "CorePrereqs.h"

#include "util/Output.h"
#include "class/IdentifierManager.h"
#include "object/ClassFactory.h"
#include "object/ObjectList.h"
#include "module/StaticallyInitializedInstance.h"

// resolve macro conflict on windows
#if defined(ORXONOX_PLATFORM_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef RegisterClass
#endif


/**
    @brief Registers the class in the framework.
    @param ClassName The name of the class
*/
#define RegisterClass(ClassName) \
    RegisterClassWithFactory(ClassName, new orxonox::ClassFactoryWithContext<ClassName>(), true)

/**
    @brief Registers the class in the framework (for classes without arguments in their constructor).
    @param ClassName The name of the class
*/
#define RegisterClassNoArgs(ClassName) \
    RegisterClassWithFactory(ClassName, new orxonox::ClassFactoryNoArgs<ClassName>(), true)

/**
    @brief Registers the class in the framework (for classes which should not be loaded through XML).
    @param ClassName The name of the class
*/
#define RegisterUnloadableClass(ClassName) \
    RegisterClassWithFactory(ClassName, new orxonox::ClassFactoryWithContext<ClassName>(), false)

/**
    @brief Registers an abstract class in the framework. Should be used in combination with inheritsFrom(base-class-identifier).
    @param ClassName The name of the class
*/
#define RegisterAbstractClass(ClassName) \
    RegisterClassWithFactory(ClassName, static_cast<ClassFactory<ClassName>*>(NULL), false)

/**
    @brief Registers the class in the framework with a given Factory.
    @param ClassName The name of the class
*/
#define RegisterClassWithFactory(ClassName, FactoryInstance, bLoadable) \
    orxonox::SI_I& _##ClassName##Identifier = (*new orxonox::SI_I(orxonox::registerClass<ClassName>(#ClassName, FactoryInstance, bLoadable)))

/**
    @brief Registers a newly created object in the framework. Has to be called at the beginning of the constructor of @a ClassName.
    @param ClassName The name of the class
*/
#define RegisterObject(ClassName) \
    if (ClassIdentifier<ClassName>::getIdentifier(#ClassName)->initializeObject(this)) \
        return; \
    else \
        ((void)0)

/**
    @brief Returns the Identifier of the given class.
    @param ClassName The name of the class
*/
#define Class(ClassName) \
    orxonox::ClassIdentifier<ClassName>::getIdentifier()


namespace orxonox
{
    /**
     * @brief Overload of registerClass() which determines T implicitly by the template argument of the ClassFactory.
     */
    template <class T>
    inline Identifier* registerClass(const std::string& name, ClassFactory<T>* factory, bool bLoadable = true)
    {
        return registerClass<T>(name, static_cast<Factory*>(factory), bLoadable);
    }

    /**
     * @brief Registers a class in the framework.
     * @param name The name of the class
     * @param factory The factory which is able to create new instances of this class
     * @param bLoadable Whether the class is allowed to be loaded through XML
     */
    template <class T>
    inline Identifier* registerClass(const std::string& name, Factory* factory, bool bLoadable = true)
    {
        orxout(verbose, context::misc::factory) << "Create entry for " << name << " in Factory." << endl;
        Identifier* identifier = ClassIdentifier<T>::getIdentifier(name);
        identifier->setFactory(factory);
        identifier->setLoadable(bLoadable);
        return identifier;
    }

    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the class
    */
    inline Identifier* ClassByString(const std::string& name)
    {
        return IdentifierManager::getInstance().getIdentifierByString(name);
    }

    /**
        @brief Returns the Identifier with a given lowercase name.
        @param name The lowercase name of the class
    */
    inline Identifier* ClassByLowercaseString(const std::string& name)
    {
        return IdentifierManager::getInstance().getIdentifierByLowercaseString(name);
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the class
    */
    inline Identifier* ClassByID(uint32_t id)
    {
        return IdentifierManager::getInstance().getIdentifierByID(id);
    }

    /**
        @brief Returns the Identifier with a given 'this' pointer.
        @note This of course only works with Identifiables.
              The only use is in conjunction with macros that don't know the class type.
        @param object Pointer to an Identifiable
    */
    template <class T>
    inline Identifier* ClassByObjectType(const T*)
    {
        return ClassIdentifier<T>::getIdentifier();
    }




    /**
     * The static initializer stores the parent classes of this identifier. The corresponding identifiers are later loaded. This prevents identifiers from
     * being used before they are completely initialized.
     */
    class _CoreExport StaticallyInitializedIdentifier : public StaticallyInitializedInstance
    {
        struct InheritsFrom
        {
            virtual ~InheritsFrom() {}
            virtual Identifier* getParent() = 0;
        };

        template <class T>
        struct InheritsFromClass : public InheritsFrom
        {
            virtual Identifier* getParent() { return Class(T); }
        };

        public:
            StaticallyInitializedIdentifier(Identifier* identifier) : identifier_(identifier) {}
            ~StaticallyInitializedIdentifier()
            {
                for (size_t i = 0; i < this->parents_.size(); ++i)
                    delete parents_[i];
            }

            virtual void load()
            {
                for (size_t i = 0; i < this->parents_.size(); ++i)
                    this->identifier_->inheritsFrom(this->parents_[i]->getParent());
            }

            inline Identifier& getIdentifier()
                { return *this->identifier_; }

            template <class T>
            inline StaticallyInitializedIdentifier& inheritsFrom()
                { this->parents_.push_back(new InheritsFromClass<T>()); return *this; }

            inline StaticallyInitializedIdentifier& virtualBase()
                { this->identifier_->setVirtualBase(true); return *this; }

        private:
            Identifier* identifier_;
            std::vector<InheritsFrom*> parents_;
    };

    typedef StaticallyInitializedIdentifier SI_I;
}

#endif /* _CoreIncludes_H__ */
