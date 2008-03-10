/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include "util/XMLIncludes.h"
#include "util/MultiTypeMath.h"
#include "util/tinyxml/ticpp.h"
#include "util/SubString.h"
#include "Functor.h"
#include "Debug.h"
#include "CoreIncludes.h"
#include "BaseObject.h"

#include "CorePrereqs.h"


#define XMLPortParam(classname, paramname, loadfunction, savefunction, xmlelement, loading) \
    orxonox::XMLPortClassParamContainer<classname>* xmlcontainer##loadfunction##savefunction = (orxonox::XMLPortClassParamContainer<classname>*)(this->getIdentifier()->getXMLPortParamContainer(paramname)); \
    if (!xmlcontainer##loadfunction##savefunction) \
    { \
        xmlcontainer##loadfunction##savefunction = new orxonox::XMLPortClassParamContainer<classname>(this->getIdentifier()->getName(), std::string(paramname), createFunctor(&classname::loadfunction), createFunctor(&classname::savefunction)); \
        this->getIdentifier()->addXMLPortParamContainer(paramname, xmlcontainer##loadfunction##savefunction); \
    } \
    xmlcontainer##loadfunction##savefunction->port(this, xmlelement, loading)

#define XMLPortParamLoadOnly(classname, paramname, loadfunction, xmlelement, loading) \
    orxonox::XMLPortClassParamContainer<classname>* xmlcontainer##loadfunction##savefunction = (orxonox::XMLPortClassParamContainer<classname>*)(this->getIdentifier()->getXMLPortParamContainer(paramname)); \
    if (!xmlcontainer##loadfunction##savefunction) \
    { \
        xmlcontainer##loadfunction##savefunction = new orxonox::XMLPortClassParamContainer<classname>(this->getIdentifier()->getName(), std::string(paramname), createFunctor(&classname::loadfunction), 0); \
        this->getIdentifier()->addXMLPortParamContainer(paramname, xmlcontainer##loadfunction##savefunction); \
    } \
    xmlcontainer##loadfunction##savefunction->port(this, xmlelement, loading)

#define XMLPortObject(classname, objectclass, sectionname, loadfunction, savefunction, xmlelement, loading, bApplyLoaderMask) \
    orxonox::XMLPortClassObjectContainer<classname, objectclass>* xmlcontainer##loadfunction##savefunction = (orxonox::XMLPortClassObjectContainer<classname, objectclass>*)(this->getIdentifier()->getXMLPortObjectContainer(sectionname)); \
    if (!xmlcontainer##loadfunction##savefunction) \
    { \
        xmlcontainer##loadfunction##savefunction = new orxonox::XMLPortClassObjectContainer<classname, objectclass>(this->getIdentifier()->getName(), std::string(sectionname), &classname::loadfunction, &classname::savefunction, bApplyLoaderMask); \
        this->getIdentifier()->addXMLPortObjectContainer(sectionname, xmlcontainer##loadfunction##savefunction); \
    } \
    xmlcontainer##loadfunction##savefunction->port(this, xmlelement, loading)


namespace orxonox
{
    // ###############################
    // ###  XMLPortParamContainer  ###
    // ###############################
    class _CoreExport XMLPortParamContainer
    {
        public:
            XMLPortParamContainer();

            inline const std::string& getName() const
                { return this->paramname_; }

            XMLPortParamContainer& description(const std::string description);
            const std::string& getDescription();

            XMLPortParamContainer& defaultValues(const MultiTypeMath& param1 = MT_null, const MultiTypeMath& param2 = MT_null, const MultiTypeMath& param3 = MT_null, const MultiTypeMath& param4 = MT_null, const MultiTypeMath& param5 = MT_null)
            {
                this->defaultValues_[0] = param1;
                this->defaultValues_[1] = param2;
                this->defaultValues_[2] = param3;
                this->defaultValues_[3] = param4;
                this->defaultValues_[4] = param5;

                return (*this);
            }

        protected:
            std::string classname_;
            std::string paramname_;
            MultiTypeMath defaultValues_[5];

        private:
            LanguageEntryLabel description_;
            bool bAddedDescription_;
            bool bAddedDefaultValues_;
    };

    template <class T>
    class XMLPortClassParamContainer : public XMLPortParamContainer
    {
        public:
            XMLPortClassParamContainer(const std::string classname, const std::string paramname, FunctorMember<T>* loadfunction, FunctorMember<T>* savefunction)
            {
                this->classname_ = classname;
                this->paramname_ = paramname;
                this->loadfunction_ = loadfunction;
                this->savefunction_ = savefunction;
            }

            XMLPortParamContainer& port(T* object, Element& xmlelement, bool loading)
            {
                if (loading)
                {
                    try
                    {
                        std::string attribute = xmlelement.GetAttribute(this->paramname_);
                        if (attribute.size() > 0)
                        {
                            SubString tokens(attribute, ",", SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');
                            if ((unsigned int)tokens.size() >= (unsigned int)this->loadfunction_->getParamCount())
                            {
                                COUT(5) << object->getLoaderIndentation() << "Loading parameter " << this->paramname_ << " in " << this->classname_ << " (objectname " << object->getName() << ") with ";
                                if (this->loadfunction_->getParamCount() == 1)
                                {
                                    COUT(5) << "1 parameter (using whole string):" << std::endl;
                                    COUT(5) << object->getLoaderIndentation() << "  " << attribute << std::endl;
                                    (*this->loadfunction_)(object, MultiTypeMath(attribute));
                                }
                                else
                                {
                                    COUT(5) << tokens.size() << " parameter (using MultiTypeMath)." << std::endl;
                                    MultiTypeMath param1 = MT_null, param2 = MT_null, param3 = MT_null, param4 = MT_null, param5 = MT_null;
                                    if (tokens.size() >= 1) param1 = tokens[0];
                                    if (tokens.size() >= 2) param2 = tokens[1];
                                    if (tokens.size() >= 3) param3 = tokens[2];
                                    if (tokens.size() >= 4) param4 = tokens[3];
                                    if (tokens.size() >= 5) param5 = tokens[4];
                                    COUT(5) << object->getLoaderIndentation() << "  " << attribute << std::endl;
                                    COUT(5) << object->getLoaderIndentation() << "  " << param1 << ", " << param2 << ", " << param3 << ", " << param4 << ", " << param5 << std::endl;

                                    (*this->loadfunction_)(object, param1, param2, param3, param4, param5);
                                }
                            }
                            else
                            {
                                COUT(2) << object->getLoaderIndentation() << "Warning: Parameter \"" << this->paramname_ << "\" in \"" << this->classname_ << "\" (objectname: " << object->getName() << ") is incomplete and couln't be loaded." << std::endl;
                            }
                        }
                    }
                    catch(ticpp::Exception& ex)
                    {
                        COUT(1) << std::endl;
                        COUT(1) << "An error occurred in XMLPort.h while loading attribute '" << this->paramname_ << "' of '" << this->classname_ << "' (objectname: " << object->getName() << ") in " << object->getLevelfile() << ":" << std::endl;
                        COUT(1) << ex.what() << std::endl;
                    }
                }
                else
                {
                    if (this->savefunction_)
                    {
//                        xmlelement.SetAttribute(this->paramname_, "...");
                    }
                }

                return (*this);
            }

        private:
            FunctorMember<T>* loadfunction_;
            FunctorMember<T>* savefunction_;
    };


    // ################################
    // ###  XMLPortObjectContainer  ###
    // ################################
    class _CoreExport XMLPortObjectContainer
    {
        public:
            XMLPortObjectContainer();

            inline const std::string& getName() const
                { return this->sectionname_; }

            XMLPortObjectContainer& description(const std::string description);
            const std::string& getDescription();
            bool identifierIsIncludedInLoaderMask(const Identifier* identifier);

        protected:
            std::string classname_;
            std::string sectionname_;
            bool bApplyLoaderMask_;

        private:
            LanguageEntryLabel description_;
            bool bAddedDescription_;
    };

    template <class T, class O>
    class XMLPortClassObjectContainer : public XMLPortObjectContainer
    {
        public:
            XMLPortClassObjectContainer(const std::string classname, const std::string sectionname, void (T::*loadfunction)(O*), const O* (T::*savefunction)(unsigned int) const, bool bApplyLoaderMask)
            {
                this->classname_ = classname;
                this->sectionname_ = sectionname;
                this->loadfunction_ = loadfunction;
                this->savefunction_ = savefunction;
                this->bApplyLoaderMask_ = bApplyLoaderMask;
            }

            XMLPortObjectContainer& port(T* object, Element& xmlelement, bool loading)
            {
                if (loading)
                {
                    try
                    {
                        Element* xmlsubelement;
                        if (this->sectionname_ != "")
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
                                            COUT(4) << object->getLoaderIndentation() << "fabricating " << child->Value() << "..." << std::endl;
                                            O* newObject = (O*)identifier->fabricate();
                                            newObject->setLoaderIndentation(object->getLoaderIndentation() + "  ");
                                            newObject->setLevel(object->getLevel());
                                            newObject->XMLPort(*child, true);
                                            COUT(4) << object->getLoaderIndentation() << "assigning " << child->Value() << " (objectname " << newObject->getName() << ") to " << this->classname_ << " (objectname " << object->getName() << ")" << std::endl;
                                            (*object.*this->loadfunction_)(newObject);
                                            COUT(5) << "  ...fabricated " << child->Value() << " (objectname " << newObject->getName() << ")." << std::endl;
                                        }
                                    }
                                    else
                                    {
                                        COUT(2) << object->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a '" << Class(O)->getName() << "'." << std::endl;
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
                        COUT(1) << "An error occurred in XMLPort.h while loading a '" << Class(O)->getName() << "' in '" << this->sectionname_ << "' of '" << this->classname_ << "' (objectname: " << object->getName() << ") in " << object->getLevelfile() << ":" << std::endl;
                        COUT(1) << ex.what() << std::endl;
                    }
                }
                else
                {
                }

                return (*this);
            }

        private:
            void     (T::*loadfunction_)(O*);
            const O* (T::*savefunction_)(unsigned int) const;
    };
}

#endif /* _XMLPort_H__ */
