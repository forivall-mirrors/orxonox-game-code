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
    RegisterClass(Level);

    Level::Level(Context* context) : BaseObject(context), Synchronisable(context), Context(context)
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
            if (LevelManager::exists())
                LevelManager::getInstance().releaseActivity(this);

            if (this->xmlfile_)
                Loader::getInstance().unload(this->xmlfile_);
        }
    }

    void Level::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Level, XMLPort, xmlelement, mode);

        XMLPortParam(Level, "gametype", setGametypeString, getGametypeString, xmlelement, mode).defaultValues("Gametype");

        XMLPortObject(Level, MeshLodInformation, "lodinformation", addLodInfo, getLodInfo, xmlelement, mode);
        XMLPortObjectExtended(Level, BaseObject, "", addObject, getObject, xmlelement, mode, true, false);
    }

    void Level::registerVariables()
    {
        registerVariable(this->xmlfilename_,            VariableDirection::ToClient, new NetworkCallback<Level>(this, &Level::networkcallback_applyXMLFile));
        registerVariable(this->name_,                   VariableDirection::ToClient, new NetworkCallback<Level>(this, &Level::changedName));
        registerVariable(this->networkTemplateNames_,   VariableDirection::ToClient, new NetworkCallback<Level>(this, &Level::networkCallbackTemplatesChanged));
    }

    void Level::networkcallback_applyXMLFile()
    {
        orxout(user_status) << "Loading level \"" << this->xmlfilename_ << "\"..." << endl;

        ClassTreeMask mask;
        mask.exclude(Class(BaseObject));
        mask.include(Class(Template));
        mask.include(Class(OverlayGroup)); // HACK to include the ChatOverlay

        this->xmlfile_ = new XMLFile(mask, this->xmlfilename_);

        Loader::getInstance().open(this->xmlfile_);
    }

    void Level::networkCallbackTemplatesChanged()
    {
        for( std::set<std::string>::iterator it = this->networkTemplateNames_.begin(); it!=this->networkTemplateNames_.end(); ++it )
        {
            assert(Template::getTemplate(*it));
            Template::getTemplate(*it)->applyOn(this);
        }
    }

    void Level::setGametypeString(const std::string& gametype)
    {
        Identifier* identifier = ClassByString(gametype);

        if (!identifier || !identifier->isA(Class(Gametype)))
        {
            orxout(internal_error) << "\"" << gametype << "\" is not a valid gametype." << endl;
            identifier = Class(Gametype);
            this->gametype_ = "Gametype";
        }
        else
            this->gametype_ = gametype;

        Gametype* rootgametype = orxonox_cast<Gametype*>(identifier->fabricate(this));
        this->setGametype(rootgametype);

        for (std::list<BaseObject*>::iterator it = this->objects_.begin(); it != this->objects_.end(); ++it)
            (*it)->setGametype(rootgametype);

        if (LevelManager::exists())
            LevelManager::getInstance().requestActivity(this);
    }


    void Level::addObject(BaseObject* object)
    {
        this->objects_.push_back(object);
        object->setGametype(this->getGametype());
        object->setLevel(this);
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

    void Level::addLodInfo(MeshLodInformation* lodInformation)
    {
        std::string meshName = lodInformation->getMeshName();
//         this->lodInformation_.insert(std::make_pair(meshName,lodInformation));
        if( this->lodInformation_.find(meshName) != this->lodInformation_.end())
            orxout(verbose, context::lod) << "replacing lod information for " << meshName << endl;
        this->lodInformation_[meshName] = lodInformation;
    }

    MeshLodInformation* Level::getLodInfo(std::string meshName) const
    {
        if(this->lodInformation_.find(meshName)!=this->lodInformation_.end())
            return this->lodInformation_.find(meshName)->second;

        return 0;
    }

    void Level::playerEntered(PlayerInfo* player)
    {
        orxout(internal_info) << "player entered level (id: " << player->getClientID() << ", name: " << player->getName() << ')' << endl;
        player->setGametype(this->getGametype());
    }

    void Level::playerLeft(PlayerInfo* player)
    {
        orxout(internal_info) << "player left level (id: " << player->getClientID() << ", name: " << player->getName() << ')' << endl;
        player->setGametype(0);
    }
}
