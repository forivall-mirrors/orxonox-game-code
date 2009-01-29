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
    @file
    @brief Declaration of the XMLPort helper classes and macros.

    XMLPort is a virtual function of every BaseObject. Every object can change this function.
    The XMLPort function gets called with an XMLElement, containing all attributes and
    subclasses the object gets from the levelfile.

    This file declares classes and macros to simplify XML-parsing.
*/

#ifndef _XMLPort_H__
#define _XMLPort_H__

#include "CorePrereqs.h"

#include <tinyxml/ticpp.h>
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/MultiType.h"
#include "XMLIncludes.h"
#include "Executor.h"
#include "CoreIncludes.h"
#include "BaseObject.h"

// ------------
// XMLPortParam

/**
    @brief Declares an XML attribute with a name, which will be set through load- and savefunctions.
    @param classname The name of the class owning this param
    @param paramname The name of the attribute
    @param loadfunction A function to set the param in the object with a given value (~a set-function)
    @param savefunction A function to get the value of the param from the object (~a get-function)
    @param xmlelement The XMLElement, you get this from the XMLPort function
    @param mode The mode (load or save), you get this from the XMLPort function

    In the XML file, a param or attribute will be set like this:
    <classname paramname="value" />

    The macro will then call loadfunction(value) to set the given value (or call savefunction() to
    write an existing value to an XML file).
*/
#define XMLPortParam(classname, paramname, loadfunction, savefunction, xmlelement, mode) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), std::string( #classname ) + "::" + #savefunction); \
    XMLPortParamGeneric(xmlcontainer##loadfunction##savefunction, classname, classname, this, paramname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode)
/**
    @brief This is the same as XMLPortParam, but you can set the template arguments needed to store the loadfunction.

    Sometimes the name of the loadfunction is ambiguous (example: setPosition(Vector3) or
    setPosition(float, float, float)). In this case, you can choose the right function by
    telling the types of the functionparams. In our example, this would be either
    > XMLPortParamTemplate(classname, paramname, loadfunction, savefunction, xmlelement, mode, Vector3);
    or
    > XMLPortParamTemplate(classname, paramname, loadfunction, savefunction, xmlelement, mode, float, float, float);
    You don't have to use this, if there exist only one function with the given name.
*/
#define XMLPortParamTemplate(classname, paramname, loadfunction, savefunction, xmlelement, mode, ...) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), std::string( #classname ) + "::" + #savefunction); \
    XMLPortParamGeneric(xmlcontainer##loadfunction##savefunction, classname, classname, this, paramname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode)

// --------------------
// XMLPortParamLoadOnly

/**
    @brief Declares an XML attribute with a name, which can be set through a loadfunction.

    This is the same as XMLPortParam, but you don't need a savefunction (get-function). Therefore,
    this param won't be saved in an XML file, but you can add the attribute manually an it will be
    loaded.

    This might be helpful in cases, when you have several options to set a value, for example the
    rotation. You can set the rotation with a quaternion, but you could also use three angles.
    When saving the object, only one of both options has to be saved; this is, where this macro helps.
*/
#define XMLPortParamLoadOnly(classname, paramname, loadfunction, xmlelement, mode) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##0##loadexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    XMLPortParamGeneric(xmlcontainer##loadfunction##0, classname, classname, this, paramname, xmlcontainer##loadfunction##0##loadexecutor, 0, xmlelement, mode)
/**
    @brief This is the same as XMLPortParamTemplate, but for load-only attributes (see XMLPortParamLoadOnly).
*/
#define XMLPortParamLoadOnlyTemplate(classname, paramname, loadfunction, xmlelement, mode, ...) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##0##loadexecutor = orxonox::createExecutor(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    XMLPortParamGeneric(xmlcontainer##loadfunction##0, classname, classname, this, paramname, xmlcontainer##loadfunction##0##loadexecutor, 0, xmlelement, mode)

// ------------------
// XMLPortParamExtern

/**
    @brief This is the same as XMLPortParam, but for attributes in an extern class.
    @param classname The name of the class owning the object owning the attribute
    @param externclass The name of the extern class (the objects class)
    @param object The name of the object of the extern class (a member of the main class)
    @param paramname The name of the attribute
    @param loadfunction The function to set the attribute inside of the member object.
    @param loadfunction The function to get the attribute from the member object

    Sometimes you'll have a member object in your class, which has it's own load- and savefunctions.
    With this macro, you can simply use them instead of writing your own functions.

    @example
    Your class is called SpaceShip and this class has an object (myPilot_) of class Pilot. Pilot has a name
    and two functions, setName(name) and getName(). Now you want an attribute "pilotname" in your
    SpaceShip class. Instead of writing wrapper functions, you can simply use the XMLPortParamExtern
    macro:
    > XMLPortParamExtern(SpaceShip, Pilot, myPilot_, "pilotname", setName, getName, xmlelement, mode);
*/
#define XMLPortParamExtern(classname, externclass, object, paramname, loadfunction, savefunction, xmlelement, mode) \
    static ExecutorMember<externclass>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor(&externclass::loadfunction), std::string( #externclass ) + "::" + #loadfunction); \
    static ExecutorMember<externclass>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&externclass::savefunction), std::string( #externclass ) + "::" + #savefunction); \
    XMLPortParamGeneric(xmlcontainer##loadfunction##savefunction, classname, externclass, object, paramname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode);
/**
    @brief This is the same as XMLPortParamTemplate, but for extern attributes (see XMLPortParamExtern).
*/
#define XMLPortParamExternTemplate(classname, externclass, object, paramname, loadfunction, savefunction, xmlelement, mode, ...) \
    static ExecutorMember<externclass>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor<externclass, __VA_ARGS__ >(&externclass::loadfunction), std::string( #externclass ) + "::" + #loadfunction); \
    static ExecutorMember<externclass>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&externclass::savefunction), std::string( #externclass ) + "::" + #savefunction); \
    XMLPortParamGeneric(xmlcontainer##loadfunction##savefunction, classname, externclass, object, paramname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode);

// -------------------
// XMLPortParamGeneric

/**
    @brief This is the generic XMLPort param macro, which is used by all six specialized macros above.
*/
#define XMLPortParamGeneric(containername, classname, objectclass, object, paramname, loadexecutor, saveexecutor, xmlelement, mode) \
    orxonox::XMLPortClassParamContainer<objectclass>* containername = (orxonox::XMLPortClassParamContainer<objectclass>*)(ClassIdentifier<classname>::getIdentifier()->getXMLPortParamContainer(paramname)); \
    if (!containername) \
    { \
        containername = new orxonox::XMLPortClassParamContainer<objectclass>(std::string(paramname), ClassIdentifier<classname>::getIdentifier(), loadexecutor, saveexecutor); \
        ClassIdentifier<classname>::getIdentifier()->addXMLPortParamContainer(paramname, containername); \
    } \
    containername->port((BaseObject*)this, object, xmlelement, mode)

// --------------------
// XMLPortObjectExtended

/**
    @brief Declares a possible sub-object that can be added in the XML file.
    @param classname The name of the class that uses this macro
    @param objectclass The baseclass of objects that can be added
    @param sectionname The name of the subsection in the XML file that encloses the sub-objects ("" means no subsection)
    @param loadfunction The function to add a new object to the class
    @param loadfunction The function to get all added objects from the class
    @param xmlelement The XMLElement (recieved through the XMLPort function)
    @param mode The mode (load/save) (received through the XMLPort function)
    @param bApplyLoaderMask If this is true, an added sub-object only gets loaded if it's class is included in the Loaders ClassTreeMask (this is usually false)
    @param bLoadBefore If this is true, the sub-cobject gets loaded (through XMLPort) BEFORE it gets added to the main class (this is usually true)

    bApplyLoaderMask is usually false for the following reason:
    If the loaders mask says, for example, "load only SpaceShips" and you added weapons to the
    SpaceShips, then the Weapons will still be loaded (which is most probably what you want).
    Of course, if there are "standalone" weapons in the level, they wont be loaded.

    If bLoadBefore, an added object already has all attributes set (like it's name). This is most
    likely the best option, so this is usually true.

    @note
    The load- and savefunctions have to follow an exactly defined protocol.
    Loadfunction:
      The loadfunction gets a pointer to the object.
      > void loadfunction(objectclass* pointer);

    Savefunction:
      The savefunction gets an index, starting with 0. For every returnvalue != 0, the savefunction
      gets called again, but with index + 1. It's the functions responsibility to do something smart
      with the index and to return 0 if all objects were returned.
      > objectclass* savefunction(unsigned int index) const;

      Possible implementation:
        objectclass* savefunction(unsigned int index) const
        {
          if (index < number_of_added_objects_)
            return my_added_objects[index];
          else
            return 0;
        }

    @example
    Possible usage of the macro:
    > XMLPortObject(SpaceShip, Weapon, "weapons", addWeapon, getWeapon, xmlelement, mode, false, true);

    Now you can add weapons through the XML file:
    <SpaceShip someattribute="..." ...>
      <weapons>
        <Weapon someattribute="..." ... />
        <Weapon someattribute="..." ... />
        <Weapon someattribute="..." ... />
      </weapons>
    </SpaceShip>

    Note that "weapons" is the subsection. This allows you to add more types of sub-objects. In our example,
    you could add pilots, blinking lights or other stuff. If you don't want a subsection, just use "" (an
    empty string). The you can add sub-objects directly into the mainclass.
*/
#define XMLPortObjectExtended(classname, objectclass, sectionname, loadfunction, savefunction, xmlelement, mode, bApplyLoaderMask, bLoadBefore) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), std::string( #classname ) + "::" + #savefunction); \
    XMLPortObjectGeneric(xmlcontainer##loadfunction##savefunction, classname, objectclass, sectionname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode, bApplyLoaderMask, bLoadBefore)
/**
    @brief This is the same as XMLPortObjectExtended, but you can specify the loadfunction by adding the param types. See XMLPortParamTemplate for more details about the types.
*/
#define XMLPortObjectExtendedTemplate(classname, objectclass, sectionname, loadfunction, savefunction, xmlelement, mode, bApplyLoaderMask, bLoadBefore, ...) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), std::string( #classname ) + "::" + #savefunction); \
    XMLPortObjectGeneric(xmlcontainer##loadfunction##savefunction, classname, objectclass, sectionname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode, bApplyLoaderMask, bLoadBefore)

// -------------
// XMLPortObject

/**
    @brief This is the same as XMLPortObjectExtended, but bApplyLoaderMask is false and bLoadBefore is true by default.
*/
#define XMLPortObject(classname, objectclass, sectionname, loadfunction, savefunction, xmlelement, mode) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), std::string( #classname ) + "::" + #savefunction); \
    XMLPortObjectGeneric(xmlcontainer##loadfunction##savefunction, classname, objectclass, sectionname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode, false, true)
/**
    @brief This is the same as XMLPortObject, but you can specify the loadfunction by adding the param types. See XMLPortParamTemplate for more details about the types.
*/
#define XMLPortObjectTemplate(classname, objectclass, sectionname, loadfunction, savefunction, xmlelement, mode, ...) \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##loadexecutor = orxonox::createExecutor(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::loadfunction), std::string( #classname ) + "::" + #loadfunction); \
    static ExecutorMember<classname>* xmlcontainer##loadfunction##savefunction##saveexecutor = orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), std::string( #classname ) + "::" + #savefunction); \
    XMLPortObjectGeneric(xmlcontainer##loadfunction##savefunction, classname, objectclass, sectionname, xmlcontainer##loadfunction##savefunction##loadexecutor, xmlcontainer##loadfunction##savefunction##saveexecutor, xmlelement, mode, false, true)

// --------------------
// XMLPortObjectGeneric

/**
    @brief Generic XMLPortObject macro, that gets called by all other XMLPortObject macros above.
*/
#define XMLPortObjectGeneric(containername, classname, objectclass, sectionname, loadexecutor, saveexecutor, xmlelement, mode, bApplyLoaderMask, bLoadBefore) \
    orxonox::XMLPortClassObjectContainer<classname, objectclass>* containername = (orxonox::XMLPortClassObjectContainer<classname, objectclass>*)(ClassIdentifier<classname>::getIdentifier()->getXMLPortObjectContainer(sectionname)); \
    if (!containername) \
    { \
        containername = new orxonox::XMLPortClassObjectContainer<classname, objectclass>(std::string(sectionname), ClassIdentifier<classname>::getIdentifier(), loadexecutor, saveexecutor, bApplyLoaderMask, bLoadBefore); \
        ClassIdentifier<classname>::getIdentifier()->addXMLPortObjectContainer(sectionname, containername); \
    } \
    containername->port(this, xmlelement, mode)


namespace orxonox
{
    // ###############################
    // ###  XMLPortParamContainer  ###
    // ###############################
    class _CoreExport XMLPortParamContainer
    {
    public:
        enum ParseResult
        {
            PR_not_started,
            PR_finished,
            PR_waiting_for_default_values
        };

        public:
            XMLPortParamContainer()
                { this->parseResult_ = PR_not_started; }
            virtual ~XMLPortParamContainer() {}

            inline const std::string& getName() const
                { return this->paramname_; }

            virtual XMLPortParamContainer& description(const std::string description) = 0;
            virtual const std::string& getDescription() = 0;

            virtual XMLPortParamContainer& defaultValue(unsigned int index, const MultiType& param) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5) = 0;

        protected:
            std::string paramname_;
            ParseResult parseResult_;
            Identifier* identifier_;
            BaseObject* owner_;
    };

    template <class T>
    class XMLPortClassParamContainer : public XMLPortParamContainer
    {
        struct ParseParams
        {
            T* object;
            Element* xmlelement;
            XMLPort::Mode mode;
        };

        public:
            XMLPortClassParamContainer(const std::string paramname, Identifier* identifier, ExecutorMember<T>* loadexecutor, ExecutorMember<T>* saveexecutor)
            {
                this->paramname_ = paramname;
                this->identifier_ = identifier;
                this->loadexecutor_ = loadexecutor;
                this->saveexecutor_ = saveexecutor;
            }

            XMLPortParamContainer& port(BaseObject* owner, T* object, Element& xmlelement, XMLPort::Mode mode)
            {
                this->owner_ = owner;
                this->parseParams_.object = object;
                this->parseParams_.xmlelement = &xmlelement;
                this->parseParams_.mode = mode;

                if (mode == XMLPort::LoadObject)
                {
                    try
                    {
                        std::string attribute = xmlelement.GetAttribute(this->paramname_);
                        if ((attribute.size() > 0) || (this->loadexecutor_->allDefaultValuesSet()))
                        {
                            COUT(5) << this->owner_->getLoaderIndentation() << "Loading parameter " << this->paramname_ << " in " << this->identifier_->getName() << " (objectname " << this->owner_->getName() << ")." << std::endl << this->owner_->getLoaderIndentation();
                            if (this->loadexecutor_->parse(object, attribute, ","))
                                this->parseResult_ = PR_finished;
                            else
                                this->parseResult_ = PR_waiting_for_default_values;
                        }
                        else
                            this->parseResult_ = PR_waiting_for_default_values;
                    }
                    catch (ticpp::Exception& ex)
                    {
                        COUT(1) << std::endl;
                        COUT(1) << "An error occurred in XMLPort.h while loading attribute '" << this->paramname_ << "' of '" << this->identifier_->getName() << "' (objectname: " << this->owner_->getName() << ") in " << this->owner_->getFilename() << ":" << std::endl;
                        COUT(1) << ex.what() << std::endl;
                    }
                }
                else
                {
                    if (this->saveexecutor_)
                    {
//                        xmlelement.SetAttribute(this->paramname_, "...");
                    }
                }

                return (*this);
            }

            XMLPortParamContainer& port(BaseObject* owner, const ParseParams& parseParams)
            {
                return this->port(owner, parseParams.object, *parseParams.xmlelement, parseParams.mode);
            }

            XMLPortParamContainer& portIfWaitingForDefaultValues(const ParseResult& result, const ParseParams& params)
            {
                if (result == PR_waiting_for_default_values)
                    return this->port(this->owner_, params);
                else
                    return (*this);
            }

            virtual XMLPortParamContainer& description(const std::string description)
                { this->loadexecutor_->setDescription(description); return (*this); }
            virtual const std::string& getDescription()
                { return this->loadexecutor_->getDescription(); }

            virtual XMLPortParamContainer& defaultValue(unsigned int index, const MultiType& param)
            {
                if (!this->loadexecutor_->defaultValueSet(index))
                    this->loadexecutor_->setDefaultValue(index, param);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1)
            {
                if (!this->loadexecutor_->defaultValueSet(0))
                    this->loadexecutor_->setDefaultValues(param1);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)))
                    this->loadexecutor_->setDefaultValues(param1, param2);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)) || (!this->loadexecutor_->defaultValueSet(2)))
                    this->loadexecutor_->setDefaultValues(param1, param2, param3);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)) || (!this->loadexecutor_->defaultValueSet(2)) || (!this->loadexecutor_->defaultValueSet(3)))
                    this->loadexecutor_->setDefaultValues(param1, param2, param3, param4);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)) || (!this->loadexecutor_->defaultValueSet(2)) || (!this->loadexecutor_->defaultValueSet(3)) || (!this->loadexecutor_->defaultValueSet(4)))
                    this->loadexecutor_->setDefaultValues(param1, param2, param3, param4, param5);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }

        private:
            ExecutorMember<T>* loadexecutor_;
            ExecutorMember<T>* saveexecutor_;
            ParseParams parseParams_;
    };


    // ################################
    // ###  XMLPortObjectContainer  ###
    // ################################
    class _CoreExport XMLPortObjectContainer
    {
        public:
            XMLPortObjectContainer()
                { this->bApplyLoaderMask_ = false; }
            virtual ~XMLPortObjectContainer() {}

            inline const std::string& getName() const
                { return this->sectionname_; }

            virtual XMLPortObjectContainer& description(const std::string description) = 0;
            virtual const std::string& getDescription() = 0;

            bool identifierIsIncludedInLoaderMask(const Identifier* identifier);

        protected:
            std::string sectionname_;
            bool bApplyLoaderMask_;
            bool bLoadBefore_;
            Identifier* identifier_;
    };

    template <class T, class O>
    class XMLPortClassObjectContainer : public XMLPortObjectContainer
    {
        public:
            XMLPortClassObjectContainer(const std::string sectionname, Identifier* identifier, ExecutorMember<T>* loadexecutor, ExecutorMember<T>* saveexecutor, bool bApplyLoaderMask, bool bLoadBefore)
            {
                this->sectionname_ = sectionname;
                this->identifier_ = identifier;
                this->loadexecutor_ = loadexecutor;
                this->saveexecutor_ = saveexecutor;
                this->bApplyLoaderMask_ = bApplyLoaderMask;
                this->bLoadBefore_ = bLoadBefore;
            }

            XMLPortObjectContainer& port(T* object, Element& xmlelement, XMLPort::Mode mode)
            {
                if (mode == XMLPort::LoadObject)
                {
                    try
                    {
                        Element* xmlsubelement;
                        if ((this->sectionname_ != "") && (this->sectionname_.size() > 0))
                            xmlsubelement = xmlelement.FirstChildElement(this->sectionname_, false);
                        else
                            xmlsubelement = &xmlelement;

                        if (xmlsubelement)
                        {
                            for (ticpp::Iterator<ticpp::Element> child = xmlsubelement->FirstChildElement(false); child != child.end(); child++)
                            {
                                Identifier* identifier = ClassByString(child->Value());
                                if (identifier)
                                {
                                    if (identifier->isA(Class(O)))
                                    {
                                        if (identifier->isLoadable())
                                        {
                                            if (this->identifierIsIncludedInLoaderMask(identifier))
                                            {
                                                try
                                                {
                                                    COUT(4) << ((BaseObject*)object)->getLoaderIndentation() << "fabricating " << child->Value() << "..." << std::endl;

                                                    BaseObject* newObject = identifier->fabricate((BaseObject*)object);
                                                    assert(newObject);
                                                    newObject->setLoaderIndentation(((BaseObject*)object)->getLoaderIndentation() + "  ");

                                                    O* castedObject = dynamic_cast<O*>(newObject);
                                                    assert(castedObject);

                                                    if (this->bLoadBefore_)
                                                    {
                                                        newObject->XMLPort(*child, XMLPort::LoadObject);
                                                        COUT(4) << ((BaseObject*)object)->getLoaderIndentation() << "assigning " << child->Value() << " (objectname " << newObject->getName() << ") to " << this->identifier_->getName() << " (objectname " << ((BaseObject*)object)->getName() << ")" << std::endl;
                                                    }
                                                    else
                                                    {
                                                        COUT(4) << ((BaseObject*)object)->getLoaderIndentation() << "assigning " << child->Value() << " (object not yet loaded) to " << this->identifier_->getName() << " (objectname " << ((BaseObject*)object)->getName() << ")" << std::endl;
                                                    }

                                                    COUT(5) << ((BaseObject*)object)->getLoaderIndentation();
                                                    (*this->loadexecutor_)(object, castedObject);

                                                    if (!this->bLoadBefore_)
                                                        newObject->XMLPort(*child, XMLPort::LoadObject);

                                                    COUT(5) << ((BaseObject*)object)->getLoaderIndentation() << "...fabricated " << child->Value() << " (objectname " << newObject->getName() << ")." << std::endl;
                                                }
                                                catch (AbortLoadingException& ex)
                                                {
                                                    COUT(1) << "An error occurred while loading object, abort loading..." << std::endl;
                                                    throw ex;
                                                }
                                                catch (std::exception& ex)
                                                {
                                                    COUT(1) << "An error occurred while loading object:" << std::endl;
                                                    COUT(1) << ex.what() << std::endl;
                                                }
                                                catch (...)
                                                {
                                                    COUT(1) << "An unknown error occurred while loading object." << std::endl;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            COUT(2) << ((BaseObject*)object)->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not loadable." << std::endl;
                                        }
                                    }
                                    else
                                    {
                                        COUT(2) << ((BaseObject*)object)->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a '" << Class(O)->getName() << "'." << std::endl;
                                    }
                                }
                                else
                                {
                                    if (this->sectionname_ != "")
                                    {
                                        COUT(2) << object->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a valid classname." << std::endl;
                                    }
                                    else
                                    {
                                        // It's probably just another subsection
                                    }
                                }
                            }
                        }
                    }
                    catch (ticpp::Exception& ex)
                    {
                        COUT(1) << std::endl;
                        COUT(1) << "An error occurred in XMLPort.h while loading a '" << Class(O)->getName() << "' in '" << this->sectionname_ << "' of '" << this->identifier_->getName() << "' (objectname: " << ((BaseObject*)object)->getName() << ") in " << object->getFilename() << ":" << std::endl;
                        COUT(1) << ex.what() << std::endl;
                    }
                }
                else
                {
                }

                return (*this);
            }

            virtual XMLPortObjectContainer& description(const std::string description)
                { this->loadexecutor_->setDescription(description); return (*this); }
            virtual const std::string& getDescription()
                { return this->loadexecutor_->getDescription(); }

        private:
            ExecutorMember<T>* loadexecutor_;
            ExecutorMember<T>* saveexecutor_;
    };
}

#endif /* _XMLPort_H__ */
