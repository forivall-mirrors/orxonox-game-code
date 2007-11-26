#ifndef _ClassFactory_H__
#define _ClassFactory_H__

#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###      ClassFactory       ###
    // ###############################
    template <class T>
    class ClassFactory : public BaseFactory
    {
        public:
            static bool create();
            BaseObject* fabricate();

        private:
            ClassFactory() {}
            ClassFactory(const ClassFactory& factory) {}
            ~ClassFactory() {}

            static T* createNewObject();
    };

    template <class T>
    bool ClassFactory<T>::create()
    {
        ClassIdentifier<T>::getIdentifier()->addFactory(new ClassFactory<T>);

        ClassIdentifier<T>::getIdentifier()->startCreatingHierarchy();
#if HIERARCHY_VERBOSE
        std::cout << "*** Create Factory -> Create Class\n";
#endif
        BaseObject* temp = ClassIdentifier<T>::getIdentifier()->fabricate();
        delete temp;
        ClassIdentifier<T>::getIdentifier()->stopCreatingHierarchy();

        return true;
    }

    template <class T>
    BaseObject* ClassFactory<T>::fabricate()
    {
        return ClassFactory<T>::createNewObject();
    }

    template <class T>
    T* ClassFactory<T>::createNewObject()
    {
        return new T;
    }
}

#endif
