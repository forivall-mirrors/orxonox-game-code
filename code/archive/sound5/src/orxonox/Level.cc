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

#include "Level.h"

#include "util/Math.h"
#include "core/CoreIncludes.h"
#include "core/Loader.h"
#include "core/Template.h"
#include "core/XMLFile.h"
#include "core/XMLPort.h"

#include "infos/PlayerInfo.h"
#include "gametypes/Gametype.h"
#include "overlays/OverlayGroup.h"
#include "LevelManager.h"

namespace orxonox
{
    CreateFactory(Level);

    Level::Level(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(Level);

        this->registerVariables();
        this->xmlfilename_ = this->getFilename();
        this->xmlfile_ = 0;
    }

    Level::~Level()
    {
        if (this->isInitialized())
        {
            if (LevelManager::getInstancePtr())
                LevelManager::getInstance().releaseActivity(this);

            if (this->xmlfile_)
                Loader::unload(this->xmlfile_);
        }
    }

    void Level::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Level, XMLPort, xmlelement, mode);

        XMLPortParam(Level, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(Level, "gametype", setGametypeString, getGametypeString, xmlelement, mode).defaultValues("Gametype");

        XMLPortObjectExtended(Level, BaseObject, "", addObject, getObject, xmlelement, mode, true, false);
    }

    void Level::registerVariables()
    {
        registerVariable(this->xmlfilename_, VariableDirection::ToClient, new NetworkCallback<Level>(this, &Level::networkcallback_applyXMLFile));
        registerVariable(this->name_,        VariableDirection::ToClient, new NetworkCallback<Level>(this, &Level::changedName));
        registerVariable(this->description_, VariableDirection::ToClient);
    }

    void Level::networkcallback_applyXMLFile()
    {
        COUT(0) << "Loading level \"" << this->xmlfilename_ << "\"..." << std::endl;

        ClassTreeMask mask;
        mask.exclude(Class(BaseObject));
        mask.include(Class(Template));
        mask.include(Class(OverlayGroup)); // HACK to include the ChatOverlay

        this->xmlfile_ = new XMLFile(mask, this->xmlfilename_);

        Loader::open(this->xmlfile_);
    }

    void Level::setGametypeString(const std::string& gametype)
    {
        Identifier* identifier = ClassByString(gametype);

        if (!identifier || !identifier->isA(Class(Gametype)))
        {
            COUT(0) << "Error: \"" << gametype << "\" is not a valid gametype." << std::endl;
            identifier = Class(Gametype);
            this->gametype_ = "Gametype";
        }
        else
            this->gametype_ = gametype;

        Gametype* rootgametype = orxonox_cast<Gametype*>(identifier->fabricate(this));
        this->setGametype(rootgametype);

        for (std::list<BaseObject*>::iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
            (*it)->setGametype(rootgametype);

        if (LevelManager::getInstancePtr())
            LevelManager::getInstance().requestActivity(this);
    }


    void Level::addObject(BaseObject* object)
    {
        this->objects_.push_back(object);
        object->setGametype(this->getGametype());
    }

    BaseObject* Level::getObject(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<BaseObject*>::const_iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void Level::playerEntered(PlayerInfo* player)
    {
        COUT(3) << "player entered level (id: " << player->getClientID() << ", name: " << player->getName() << ')' << std::endl;
        player->setGametype(this->getGametype());
    }

    void Level::playerLeft(PlayerInfo* player)
    {
        COUT(3) << "player left level (id: " << player->getClientID() << ", name: " << player->getName() << ')' << std::endl;
        player->setGametype(0);
    }
}
