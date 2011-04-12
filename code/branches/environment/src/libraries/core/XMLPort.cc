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

#include "XMLPort.h"

#include "Loader.h"
#include "Namespace.h"

namespace orxonox
{
    // ################################
    // ###  XMLPortObjectContainer  ###
    // ################################
    bool XMLPortObjectContainer::identifierIsIncludedInLoaderMask(const Identifier* identifier)
    {
        return ((!this->bApplyLoaderMask_) || identifier->isA(ClassIdentifier<Namespace>::getIdentifier()) || Loader::currentMask_s.isIncluded(identifier));
    }

    XMLPortObjectContainer& XMLPortObjectContainer::port(BaseObject* object, Element& xmlelement, XMLPort::Mode mode)
    {
        if ((mode == XMLPort::LoadObject) || (mode == XMLPort::ExpandObject))
        {
            try
            {
                Element* xmlsubelement;
                if (!this->sectionname_.empty())
                    xmlsubelement = xmlelement.FirstChildElement(this->sectionname_, false);
                else
                    xmlsubelement = &xmlelement;

                if (!xmlsubelement)
                    return (*this);

                for (ticpp::Iterator<ticpp::Element> child = xmlsubelement->FirstChildElement(false); child != child.end(); child++)
                {
                    Identifier* identifier = Identifier::getIdentifierByString(child->Value());
                    if (!identifier)
                    {
                        if (!this->sectionname_.empty())
                        {
                            COUT(2) << object->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a valid classname." << std::endl;
                        }
                        else
                        {
                            // It's probably just another subsection
                        }
                        continue;
                    }
                    if (!identifier->isA(objectIdentifier_))
                    {
                        COUT(2) << object->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not a '" << objectIdentifier_->getName() << "'." << std::endl;
                        continue;
                    }
                    if (!identifier->isLoadable())
                    {
                        COUT(2) << object->getLoaderIndentation() << "Warning: '" << child->Value() << "' is not loadable." << std::endl;
                        continue;
                    }
                    if (!this->identifierIsIncludedInLoaderMask(identifier))
                        continue;

                    try
                    {
                        COUT(4) << object->getLoaderIndentation() << "fabricating " << child->Value() << "..." << std::endl;

                        BaseObject* newObject = identifier->fabricate(object);
                        newObject->setLoaderIndentation(object->getLoaderIndentation() + "  ");

                        if (this->bLoadBefore_)
                        {
                            newObject->XMLPort(*child, XMLPort::LoadObject);
                            COUT(4) << object->getLoaderIndentation() << "assigning " << child->Value() << " (objectname " << newObject->getName() << ") to " << this->identifier_->getName() << " (objectname " << static_cast<BaseObject*>(object)->getName() << ')' << std::endl;
                        }
                        else
                        {
                            COUT(4) << object->getLoaderIndentation() << "assigning " << child->Value() << " (object not yet loaded) to " << this->identifier_->getName() << " (objectname " << static_cast<BaseObject*>(object)->getName() << ')' << std::endl;
                        }

                        COUT(5) << object->getLoaderIndentation();

                        this->callLoadExecutor(object, newObject);

                        if (!this->bLoadBefore_)
                            newObject->XMLPort(*child, XMLPort::LoadObject);

                        COUT(5) << object->getLoaderIndentation() << "...fabricated " << child->Value() << " (objectname " << newObject->getName() << ")." << std::endl;
                    }
                    catch (AbortLoadingException& ex)
                    {
                        COUT(1) << "An error occurred while loading object, abort loading..." << std::endl;
                        throw ex;
                    }
                    catch (...)
                    {
                        COUT(1) << "An error occurred while loading object:" << std::endl;
                        COUT(1) << Exception::handleMessage() << std::endl;
                    }
                }
            }
            catch (ticpp::Exception& ex)
            {
                COUT(1) << std::endl;
                COUT(1) << "An error occurred in XMLPort.h while loading a '" << objectIdentifier_->getName() << "' in '" << this->sectionname_ << "' of '" << this->identifier_->getName() << "' (objectname: " << object->getName() << ") in " << object->getFilename() << ':' << std::endl;
                COUT(1) << ex.what() << std::endl;
            }
        }
        else if (mode == XMLPort::SaveObject)
        {
        }

        return (*this);
    }
}
