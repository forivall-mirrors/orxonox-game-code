/*!
    @file ClassFactory.h
    @brief Definition of the ClassFactory class

    The ClassFactory is able to create new objects of a specific class.
*/

#ifndef _ClassFactory_H__
#define _ClassFactory_H__

#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###      ClassFactory       ###
    // ###############################
    //! The ClassFactory is able to create new objects of a specific class.
    template <class T>
    class ClassFactory : public BaseFactory
    {
        public:
            static bool create();
            BaseObject* fabricate();

        private:
            ClassFactory() {}                               // Don't create
            ClassFactory(const ClassFactory& factory) {}    // Don't copy
            ~ClassFactory() {}                              // Don't delete

            static T* createNewObject();
    };

    /**
        @brief Adds the ClassFactory to the Identifier of the same type and creates a new object to retrieve the parents.
        @return True, because the compiler only allows assignments before main()
    */
    template <class T>
    bool ClassFactory<T>::create()
    {
        // Add the ClassFactory to the Classidentifier of type T
        ClassIdentifier<T>::getIdentifier()->addFactory(new ClassFactory<T>);

        // To create the new branch of the class-hierarchy, we create a new object and delete it afterwards.
        ClassIdentifier<T>::getIdentifier()->startCreatingHierarchy();
#if HIERARCHY_VERBOSE
        std::cout << "*** Create Factory -> Create Class\n";
#endif
        BaseObject* temp = ClassIdentifier<T>::getIdentifier()->fabricate();
        delete temp;
        ClassIdentifier<T>::getIdentifier()->stopCreatingHierarchy();

        return true;
    }

    /**
        @brief Creates and returns a new object of class T.
        @return The new object
    */
    template <class T>
    BaseObject* ClassFactory<T>::fabricate()
    {
        return ClassFactory<T>::createNewObject();
    }

    /**
        @brief Creates and returns a new object of class T; this is a wrapper for the new operator.
        @return The new object
    */
    template <class T>
    T* ClassFactory<T>::createNewObject()
    {
        return new T;
    }
}

#endif
