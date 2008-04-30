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

#ifndef _XMLPort_H__
#define _XMLPort_H__

#include "CorePrereqs.h"

#include "util/XMLIncludes.h"
#include "util/MultiTypeMath.h"
#include "tinyxml/ticpp.h"
#include "Executor.h"
#include "Debug.h"
#include "CoreIncludes.h"
#include "BaseObject.h"


#define XMLPortParam(classname, paramname, loadfunction, savefunction, xmlelement, mode) \
    XMLPortParamGeneric(xmlcontainer##loadfunction##savefunction, classname, paramname, orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), #loadfunction), orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), #savefunction), xmlelement, mode)
#define XMLPortParam_Template(classname, paramname, loadtemplate, loadfunction, savetemplate, savefunction, xmlelement, mode) \
    XMLPortParamGeneric(xmlcontainer##loadfunction##savefunction, classname, paramname, orxonox::createExecutor(orxonox::createFunctor loadtemplate (&classname::loadfunction), #loadfunction), orxonox::createExecutor(orxonox::createFunctor savetemplate (&classname::savefunction), #savefunction), xmlelement, mode)

#define XMLPortParamLoadOnly(classname, paramname, loadfunction, xmlelement, mode) \
    XMLPortParamGeneric(xmlcontainer##loadfunction##0, classname, paramname, orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), #loadfunction), 0, xmlelement, mode)
#define XMLPortParamLoadOnly_Template(classname, paramname, loadtemplate, loadfunction, xmlelement, mode) \
    XMLPortParamGeneric(xmlcontainer##loadfunction##0, classname, paramname, orxonox::createExecutor(orxonox::createFunctor loadtemplate (&classname::loadfunction), #loadfunction), 0, xmlelement, mode)

#define XMLPortParamGeneric(containername, classname, paramname, loadexecutor, saveexecutor, xmlelement, mode) \
    orxonox::XMLPortClassParamContainer<classname>* containername = (orxonox::XMLPortClassParamContainer<classname>*)(this->getIdentifier()->getXMLPortParamContainer(paramname)); \
    if (!containername) \
    { \
        containername = new orxonox::XMLPortClassParamContainer<classname>(std::string(paramname), loadexecutor, saveexecutor); \
        this->getIdentifier()->addXMLPortParamContainer(paramname, containername); \
    } \
    containername->port(this, xmlelement, mode)


#define XMLPortObject(classname, objectclass, sectionname, loadfunction, savefunction, xmlelement, mode, bApplyLoaderMask, bLoadBefore) \
    XMLPortObjectGeneric(xmlcontainer##loadfunction##savefunction, classname, objectclass, sectionname, orxonox::createExecutor(orxonox::createFunctor(&classname::loadfunction), #loadfunction), orxonox::createExecutor(orxonox::createFunctor(&classname::savefunction), #savefunction), xmlelement, mode, bApplyLoaderMask, bLoadBefore)
#define XMLPortObject_Template(classname, objectclass, sectionname, loadtemplate, loadfunction, savetemplate, savefunction, xmlelement, mode, bApplyLoaderMask, bLoadBefore) \
    XMLPortObjectGeneric(xmlcontainer##loadfunction##savefunction, classname, objectclass, sectionname, orxonox::createExecutor(orxonox::createFunctor loadtemplate (&classname::loadfunction), #loadfunction), orxonox::createExecutor(orxonox::createFunctor savetemplate (&classname::savefunction), #savefunction), xmlelement, mode, bApplyLoaderMask, bLoadBefore)

#define XMLPortObjectGeneric(containername, classname, objectclass, sectionname, loadexecutor, saveexecutor, xmlelement, mode, bApplyLoaderMask, bLoadBefore) \
    orxonox::XMLPortClassObjectContainer<classname, objectclass>* containername = (orxonox::XMLPortClassObjectContainer<classname, objectclass>*)(this->getIdentifier()->getXMLPortObjectContainer(sectionname)); \
    if (!containername) \
    { \
        containername = new orxonox::XMLPortClassObjectContainer<classname, objectclass>(std::string(sectionname), loadexecutor, saveexecutor, bApplyLoaderMask, bLoadBefore); \
        this->getIdentifier()->addXMLPortObjectContainer(sectionname, containername); \
    } \
    containername->port(this, xmlelement, mode)


namespace orxonox
{

#ifndef _XMLPort_Mode__
#define _XMLPort_Mode__
    namespace XMLPort
    {
        enum Mode
        {
            LoadObject,
            SaveObject
        };
    }
#endif

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

            virtual XMLPortParamContainer& defaultValue(unsigned int index, const MultiTypeMath& param) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4) = 0;
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5) = 0;

        protected:
            std::string paramname_;
            ParseResult parseResult_;

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
            XMLPortClassParamContainer(const std::string paramname, ExecutorMember<T>* loadexecutor, ExecutorMember<T>* saveexecutor)
            {
                this->paramname_ = paramname;
                this->loadexecutor_ = loadexecutor;
                this->saveexecutor_ = saveexecutor;
            }

            XMLPortParamContainer& port(T* object, Element& xmlelement, XMLPort::Mode mode)
            {
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
                            COUT(5) << ((BaseObject*)object)->getLoaderIndentation() << "Loading parameter " << this->paramname_ << " in " << object->getIdentifier()->getName() << " (objectname " << ((BaseObject*)object)->getName() << ")." << std::endl << ((BaseObject*)object)->getLoaderIndentation();
                            if (this->loadexecutor_->parse(object, attribute, ","))
                                this->parseResult_ = PR_finished;
                            else
                                this->parseResult_ = PR_waiting_for_default_values;
                        }
                    }
                    catch(ticpp::Exception& ex)
                    {
                        COUT(1) << std::endl;
                        COUT(1) << "An error occurred in XMLPort.h while loading attribute '" << this->paramname_ << "' of '" << object->getIdentifier()->getName() << "' (objectname: " << ((BaseObject*)object)->getName() << ") in " << ((BaseObject*)object)->getLevelfile() << ":" << std::endl;
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

            XMLPortParamContainer& port(const ParseParams& parseParams)
            {
                return this->port(parseParams.object, *parseParams.xmlelement, parseParams.mode);
            }

            XMLPortParamContainer& portIfWaitingForDefaultValues(const ParseResult& result, const ParseParams& params)
            {
                if (result == PR_waiting_for_default_values)
                    return this->port(params);
                else
                    return (*this);
            }

            virtual XMLPortParamContainer& description(const std::string description)
                { this->loadexecutor_->setDescription(description); return (*this); }
            virtual const std::string& getDescription()
                { return this->loadexecutor_->getDescription(); }

            virtual XMLPortParamContainer& defaultValue(unsigned int index, const MultiTypeMath& param)
            {
                if (!this->loadexecutor_->defaultValueSet(index))
                    this->loadexecutor_->setDefaultValue(index, param);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1)
            {
                if (!this->loadexecutor_->defaultValueSet(0))
                    this->loadexecutor_->setDefaultValues(param1);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)))
                    this->loadexecutor_->setDefaultValues(param1, param2);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)) || (!this->loadexecutor_->defaultValueSet(2)))
                    this->loadexecutor_->setDefaultValues(param1, param2, param3);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4)
            {
                if ((!this->loadexecutor_->defaultValueSet(0)) || (!this->loadexecutor_->defaultValueSet(1)) || (!this->loadexecutor_->defaultValueSet(2)) || (!this->loadexecutor_->defaultValueSet(3)))
                    this->loadexecutor_->setDefaultValues(param1, param2, param3, param4);
                return this->portIfWaitingForDefaultValues(this->parseResult_, this->parseParams_);
            }
            virtual XMLPortParamContainer& defaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5)
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
    };

    template <class T, class O>
    class XMLPortClassObjectContainer : public XMLPortObjectContainer
    {
        public:
            XMLPortClassObjectContainer(const std::string sectionname, ExecutorMember<T>* loadexecutor, ExecutorMember<T>* saveexecutor, bool bApplyLoaderMask, bool bLoadBefore)
            {
                this->sectionname_ = sectionname;
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
                                Identifier* identifier = ID(child->Value());
                                if (identifier)
                                {
                                    if (identifier->isA(Class(O)))
                                    {
                                        if (this->identifierIsIncludedInLoaderMask(identifier))
                                        {
                                            COUT(4) << ((BaseObject*)object)->getLoaderIndentation() << "fabricating " << child->Value() << "..." << std::endl;

                                            BaseObject* newObject = identifier->fabricate();
                                            newObject->setLoaderIndentation(((BaseObject*)object)->getLoaderIndentation() + "  ");
                                            newObject->setLevel(((BaseObject*)object)->getLevel());
                                            newObject->setNamespace(((BaseObject*)object)->getNamespace());

                                            if (this->bLoadBefore_)
                                            {
                                                newObject->XMLPort(*child, XMLPort::LoadObject);
                                                COUT(4) << ((BaseObject*)object)->getLoaderIndentation() << "assigning " << child->Value() << " (objectname " << newObject->getName() << ") to " << object->getIdentifier()->getName() << " (objectname " << ((BaseObject*)object)->getName() << ")" << std::endl;
                                            }
                                            else
                                            {
                                                COUT(4) << ((BaseObject*)object)->getLoaderIndentation() << "assigning " << child->Value() << " (object not yet loaded) to " << object->getIdentifier()->getName() << " (objectname " << ((BaseObject*)object)->getName() << ")" << std::endl;
                                            }

                                            COUT(5) << ((BaseObject*)object)->getLoaderIndentation();
                                            (*this->loadexecutor_)(object, newObject);

                                            if (!this->bLoadBefore_)
                                                newObject->XMLPort(*child, XMLPort::LoadObject);

                                            COUT(5) << ((BaseObject*)object)->getLoaderIndentation() << "...fabricated " << child->Value() << " (objectname " << newObject->getName() << ")." << std::endl;
                                        }
                                    }
                                    else
                                    {
                                        COUT(2) << ((BaseObject*)object)->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a '" << Class(O)->getName() << "'." << std::endl;
                                    }
                                }
                                else
                                {
                                    COUT(2) << object->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a valid classname." << std::endl;
                                }
                            }
                        }
                    }
                    catch(ticpp::Exception& ex)
                    {
                        COUT(1) << std::endl;
                        COUT(1) << "An error occurred in XMLPort.h while loading a '" << Class(O)->getName() << "' in '" << this->sectionname_ << "' of '" << object->getIdentifier()->getName() << "' (objectname: " << ((BaseObject*)object)->getName() << ") in " << object->getLevelfile() << ":" << std::endl;
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
