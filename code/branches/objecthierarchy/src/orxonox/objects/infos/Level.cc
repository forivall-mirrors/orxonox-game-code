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
#include "Level.h"

#include <OgreSceneManager.h>
#include <OgreLight.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Core.h"
#include "core/ConsoleCommand.h"
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "core/Template.h"

#include "GraphicsEngine.h"
#include "Settings.h"
#include "PlayerInfo.h"

#include "util/Math.h"

namespace orxonox
{
    SetConsoleCommand(Level, listPlayers, true);

    CreateFactory(Level);

    Level::Level()
    {
        RegisterObject(Level);

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

        COUT(0) << "created Level" << std::endl;
    }

    Level* Level::getActiveLevel()
    {
        for (ObjectList<Level>::iterator it = ObjectList<Level>::begin(); it != ObjectList<Level>::end(); ++it)
            if (it->isActive())
                return (*it);

        return 0;
    }

    PlayerInfo* Level::getClient(unsigned int clientID)
    {
        Level* level = Level::getActiveLevel();

        if (level)
        {
            std::map<unsigned int, PlayerInfo*>::const_iterator it = level->clients_.find(clientID);
            if (it != level->clients_.end())
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

    void Level::listPlayers()
    {
        Level* level = Level::getActiveLevel();

        if (level->getGametype())
        {
            for (std::set<PlayerInfo*>::const_iterator it = level->getGametype()->getPlayers().begin(); it != level->getGametype()->getPlayers().end(); ++it)
                COUT(0) << "ID: " << (*it)->getClientID() << ", Name: " << (*it)->getName() << std::endl;
        }
        else
        {
            for (ObjectList<PlayerInfo>::iterator it = ObjectList<PlayerInfo>::begin(); it != ObjectList<PlayerInfo>::end(); ++it)
                COUT(0) << "ID: " << (*it)->getClientID() << ", Name: " << (*it)->getName() << std::endl;
        }
    }

    void Level::clientConnected(unsigned int clientID)
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

    void Level::clientDisconnected(unsigned int clientID)
    {
        COUT(0) << "client disconnected" << std::endl;

        // remove from clients-map
        PlayerInfo* player = this->clients_[clientID];
        this->clients_.erase(clientID);

        // delete PlayerInfo instance
        delete player;
    }

    void Level::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Level, XMLPort, xmlelement, mode);

        XMLPortParam(Level, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(Level, "gametype", setGametypeString, getGametypeString, xmlelement, mode).defaultValues("Gametype");
        XMLPortParam(Level, "skybox", setSkybox, getSkybox, xmlelement, mode);
        XMLPortParam(Level, "ambientlight", setAmbientLight, getAmbientLight, xmlelement, mode).defaultValues(ColourValue(0.2, 0.2, 0.2, 1));

        this->xmlfile_ = this->getFilename();
    }

    void Level::registerVariables()
    {
        REGISTERSTRING(this->xmlfile_,     network::direction::toclient, new network::NetworkCallback<Level>(this, &Level::networkcallback_applyXMLFile));
        REGISTERSTRING(this->name_,        network::direction::toclient, new network::NetworkCallback<Level>(this, &Level::changedName));
        REGISTERSTRING(this->description_, network::direction::toclient);
        REGISTERSTRING(this->skybox_,      network::direction::toclient, new network::NetworkCallback<Level>(this, &Level::networkcallback_applySkybox));
        REGISTERDATA(this->ambientLight_,  network::direction::toclient, new network::NetworkCallback<Level>(this, &Level::networkcallback_applyAmbientLight));
    }

    void Level::networkcallback_applyXMLFile()
    {
        COUT(0) << "Loading level \"" << this->xmlfile_ << "\"..." << std::endl;

        ClassTreeMask mask;
        mask.exclude(Class(BaseObject));
        mask.include(Class(Template));

        XMLFile* file = new XMLFile(Settings::getDataPath() + this->xmlfile_, mask);

        Loader::open(file);
    }

    void Level::setSkybox(const std::string& skybox)
    {
        if (Core::showsGraphics())
            if (GraphicsEngine::getInstance().getLevelSceneManager())
                GraphicsEngine::getInstance().getLevelSceneManager()->setSkyBox(true, skybox);

        this->skybox_ = skybox;
    }

    void Level::setAmbientLight(const ColourValue& colour)
    {
        if (Core::showsGraphics())
            GraphicsEngine::getInstance().getLevelSceneManager()->setAmbientLight(colour);

        this->ambientLight_ = colour;
    }

    void Level::setGametypeString(const std::string& gametype)
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
