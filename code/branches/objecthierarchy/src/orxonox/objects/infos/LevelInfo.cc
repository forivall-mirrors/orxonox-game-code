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

#include "OrxonoxStableHeaders.h"
#include "LevelInfo.h"

#include <OgreSceneManager.h>
#include <OgreLight.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Core.h"
#include "core/ConsoleCommand.h"
#include "core/Loader.h"
#include "core/Template.h"

#include "GraphicsEngine.h"
#include "Settings.h"
#include "PlayerInfo.h"

#include "util/Math.h"

namespace orxonox
{
    SetConsoleCommand(LevelInfo, listPlayers, true);

    CreateFactory(LevelInfo);

    LevelInfo::LevelInfo()
    {
        RegisterObject(LevelInfo);

        this->rootGametype_ = 0;
        this->registerVariables();

        // test test test
        {
            Ogre::Light* light;
            light = GraphicsEngine::getInstance().getLevelSceneManager()->createLight("Light0");
            light->setType(Ogre::Light::LT_DIRECTIONAL);
            light->setDiffuseColour(ColourValue(1.0, 0.9, 0.6, 1.0));
            light->setSpecularColour(ColourValue(1.0, 0.9, 0.6, 1.0));
            light->setDirection(1, -0.2, 0.2);
        }
        // test test test

        COUT(0) << "created LevelInfo" << std::endl;
    }

    LevelInfo* LevelInfo::getActiveLevelInfo()
    {
        for (ObjectList<LevelInfo>::iterator it = ObjectList<LevelInfo>::begin(); it != ObjectList<LevelInfo>::end(); ++it)
            if (it->isActive())
                return (*it);

        return 0;
    }

    PlayerInfo* LevelInfo::getClient(unsigned int clientID)
    {
        LevelInfo* levelinfo = LevelInfo::getActiveLevelInfo();

        if (levelinfo)
        {
            std::map<unsigned int, PlayerInfo*>::const_iterator it = levelinfo->clients_.find(clientID);
            if (it != levelinfo->clients_.end())
                return it->second;
        }
        else
        {
            for (ObjectList<PlayerInfo>::iterator it = ObjectList<PlayerInfo>::begin(); it != ObjectList<PlayerInfo>::end(); ++it)
                if (it->getClientID() == clientID)
                    return (*it);
        }
        return 0;
    }

    void LevelInfo::listPlayers()
    {
        LevelInfo* levelinfo = LevelInfo::getActiveLevelInfo();

        if (levelinfo->getGametype())
        {
            for (std::set<PlayerInfo*>::const_iterator it = levelinfo->getGametype()->getPlayers().begin(); it != levelinfo->getGametype()->getPlayers().end(); ++it)
                COUT(0) << "ID: " << (*it)->getClientID() << ", Name: " << (*it)->getName() << std::endl;
        }
        else
        {
            for (ObjectList<PlayerInfo>::iterator it = ObjectList<PlayerInfo>::begin(); it != ObjectList<PlayerInfo>::end(); ++it)
                COUT(0) << "ID: " << (*it)->getClientID() << ", Name: " << (*it)->getName() << std::endl;
        }
    }

    void LevelInfo::clientConnected(unsigned int clientID)
    {
        COUT(0) << "client connected" << std::endl;

        // create new PlayerInfo instance
        PlayerInfo* player = new PlayerInfo();
        player->setGametype(this->getGametype());
        player->setClientID(clientID);

        // add to clients-map
        assert(!this->clients_[clientID]);
        this->clients_[clientID] = player;
    }

    void LevelInfo::clientDisconnected(unsigned int clientID)
    {
        COUT(0) << "client disconnected" << std::endl;

        // remove from clients-map
        PlayerInfo* player = this->clients_[clientID];
        this->clients_.erase(clientID);

        // delete PlayerInfo instance
        delete player;
    }

    void LevelInfo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LevelInfo, XMLPort, xmlelement, mode);

        XMLPortParam(LevelInfo, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(LevelInfo, "gametype", setGametypeString, getGametypeString, xmlelement, mode).defaultValues("Gametype");
        XMLPortParam(LevelInfo, "skybox", setSkybox, getSkybox, xmlelement, mode);
        XMLPortParam(LevelInfo, "ambientlight", setAmbientLight, getAmbientLight, xmlelement, mode).defaultValues(ColourValue(0.2, 0.2, 0.2, 1));

        this->levelfile_ = this->getLevelfile();
    }

    void LevelInfo::registerVariables()
    {
        REGISTERSTRING(this->levelfile_,    network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::applyLevel));
        REGISTERSTRING(this->name_,         network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::changedName));
        REGISTERSTRING(this->description_,  network::direction::toclient);
        REGISTERSTRING(this->skybox_,       network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::applySkybox));
        REGISTERDATA(this->ambientLight_, network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::applyAmbientLight));
    }

    void LevelInfo::applyLevel()
    {
        COUT(0) << "Loading level \"" << this->levelfile_ << "\"..." << std::endl;

        ClassTreeMask mask;
        mask.exclude(Class(BaseObject));
        mask.include(Class(Template));

        Level* level = new Level(Settings::getDataPath() + this->levelfile_, mask);

        Loader::open(level);
    }

    void LevelInfo::setSkybox(const std::string& skybox)
    {
        if (Core::showsGraphics())
            if (GraphicsEngine::getInstance().getLevelSceneManager())
                GraphicsEngine::getInstance().getLevelSceneManager()->setSkyBox(true, skybox);

        this->skybox_ = skybox;
    }

    void LevelInfo::setAmbientLight(const ColourValue& colour)
    {
        if (Core::showsGraphics())
            GraphicsEngine::getInstance().getLevelSceneManager()->setAmbientLight(colour);

        this->ambientLight_ = colour;
    }

    void LevelInfo::setGametypeString(const std::string& gametype)
    {
        Identifier* identifier = ClassByString(gametype);
        if (identifier)
        {
            this->gametype_ = gametype;
            this->gametypeIdentifier_ = identifier;
            this->rootGametype_ = this->gametypeIdentifier_.fabricate();
            this->getConnectedClients();
        }
    }
}
