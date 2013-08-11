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

#include "CoreIncludes.h"
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
                    Identifier* identifier = ClassByString(child->Value());
                    if (!identifier)
                    {
                        if (!this->sectionname_.empty())
                        {
                            orxout(internal_warning, context::xml) << object->getLoaderIndentation() << "'" << child->Value() << "' is not a valid classname." << endl;
                        }
                        else
                        {
                            // It's probably just another subsection
                        }
                        continue;
                    }
                    if (!identifier->isA(objectIdentifier_))
                    {
                        orxout(internal_warning, context::xml) << object->getLoaderIndentation() << "'" << child->Value() << "' is not a '" << objectIdentifier_->getName() << "'." << endl;
                        continue;
                    }
                    if (!identifier->isLoadable())
                    {
                        orxout(internal_warning, context::xml) << object->getLoaderIndentation() << "'" << child->Value() << "' is not loadable." << endl;
                        continue;
                    }
                    if (!this->identifierIsIncludedInLoaderMask(identifier))
                        continue;

                    try
                    {
                        orxout(verbose, context::xml) << object->getLoaderIndentation() << "fabricating " << child->Value() << "..." << endl;

                        BaseObject* newObject = orxonox_cast<BaseObject*>(identifier->fabricate(object->getContext()));
                        newObject->setLoaderIndentation(object->getLoaderIndentation() + "  ");

                        if (this->bLoadBefore_)
                        {
                            newObject->XMLPort(*child, XMLPort::LoadObject);
                            orxout(verbose, context::xml) << object->getLoaderIndentation() << "assigning " << child->Value() << " (objectname " << newObject->getName() << ") to " << this->identifier_->getName() << " (objectname " << static_cast<BaseObject*>(object)->getName() << ')' << endl;
                        }
                        else
                        {
                            orxout(verbose, context::xml) << object->getLoaderIndentation() << "assigning " << child->Value() << " (object not yet loaded) to " << this->identifier_->getName() << " (objectname " << static_cast<BaseObject*>(object)->getName() << ')' << endl;
                        }

                        this->callLoadExecutor(object, newObject);

                        if (!this->bLoadBefore_)
                            newObject->XMLPort(*child, XMLPort::LoadObject);

                        orxout(verbose, context::xml) << object->getLoaderIndentation() << "fabricated " << child->Value() << " (objectname " << newObject->getName() << ")." << endl;
                    }
                    catch (AbortLoadingException& ex)
                    {
                        orxout(internal_error, context::xml) << "An error occurred while loading object, abort loading..." << endl;
                        throw ex;
                    }
                    catch (...)
                    {
                        orxout(internal_error, context::xml) << "An error occurred while loading object:" << endl;
                        orxout(internal_error, context::xml) << Exception::handleMessage() << endl;
                    }
                }
            }
            catch (ticpp::Exception& ex)
            {
                orxout(internal_error, context::xml) << endl;
                orxout(internal_error, context::xml) << "An error occurred in XMLPort.h while loading a '" << objectIdentifier_->getName() << "' in '" << this->sectionname_ << "' of '" << this->identifier_->getName() << "' (objectname: " << object->getName() << ") in " << object->getFilename() << ':' << endl;
                orxout(internal_error, context::xml) << ex.what() << endl;
            }
        }
        else if (mode == XMLPort::SaveObject)
        {
        }

        return (*this);
    }
}
