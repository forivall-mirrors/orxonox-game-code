#ifndef _ClassHierarchy_H__
#define _ClassHierarchy_H__

#include <string>
#include <iostream>

// DONE AND TESTED:
// - build class hierarchy
// - isA, isChildOf, ...
// - insert into class-lists
// - ClassIdentifier
// - BaseIdentifier
// - Factory

// IN WORK:

// TO DO:
// - iterate through lists


namespace orxonox
{
    // ##### ClassHierarchy #####
    template <class T>
    class ClassIdentifier;

    class ClassHierarchy
    {
        template <class T>
        friend class ClassIdentifier;

        public:
            static ClassHierarchy* getSingleton();
            bool isCreatingHierarchy() { return (this->hierarchyCreatingCounter_ > 0); }

        private:
            ClassHierarchy();
            ClassHierarchy(const ClassHierarchy& hierarchy);
            ~ClassHierarchy();
            void startCreatingHierarchy() { this->hierarchyCreatingCounter_++; std::cout << "*** Increased Hierarchy-Creating-Counter to " << this->hierarchyCreatingCounter_ << "\n"; }
            void stopCreatingHierarchy() { this->hierarchyCreatingCounter_--; std::cout << "*** Decreased Hierarchy-Creating-Counter to " << this->hierarchyCreatingCounter_ << "\n"; }

            static ClassHierarchy* pointer_;
            int hierarchyCreatingCounter_;
    };


    // ##### Macros #####
    #define registerRootObject(ClassName) \
        std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && !this->getParents()) \
            this->setParents(new IdentifierList()); \
        if (this->getIdentifier()) \
            this->getIdentifier()->removeObject(this); \
        this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, true, false)); \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && this->getParents()) \
            this->getParents()->add(this->getIdentifier()); \
        this->getIdentifier()->addObject(this)

    #define registerAbstractRootObject(ClassName) \
        std::cout << "*** Register Root-Object: " << #ClassName << "\n"; \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && !this->getParents()) \
            this->setParents(new IdentifierList()); \
        if (this->getIdentifier()) \
            this->getIdentifier()->removeObject(this); \
        this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, true, true)); \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && this->getParents()) \
            this->getParents()->add(this->getIdentifier()); \
        this->getIdentifier()->addObject(this)

    #define registerObject(ClassName) \
        std::cout << "*** Register Object: " << #ClassName << "\n"; \
        this->getIdentifier()->removeObject(this); \
        this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, false, false)); \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && this->getParents()) \
            this->getParents()->add(this->getIdentifier()); \
        this->getIdentifier()->addObject(this)

    #define registerAbstractObject(ClassName) \
        std::cout << "*** Register Object: " << #ClassName << "\n"; \
        this->getIdentifier()->removeObject(this); \
        this->setIdentifier(ClassIdentifier<ClassName>::registerClass(this->getParents(), #ClassName, false, true)); \
        if (ClassHierarchy::getSingleton()->isCreatingHierarchy() && this->getParents()) \
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
}

#endif
