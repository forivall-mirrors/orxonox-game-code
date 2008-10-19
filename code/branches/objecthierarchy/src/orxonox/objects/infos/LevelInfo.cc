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

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/Core.h"

#include "GraphicsEngine.h"

namespace orxonox
{
    CreateFactory(LevelInfo);

    LevelInfo::LevelInfo()
    {
        RegisterObject(LevelInfo);

        this->gametypeInstance_ = 0;
        this->registerVariables();

        COUT(0) << "created LevelInfo" << std::endl;
    }

    void LevelInfo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LevelInfo, XMLPort, xmlelement, mode);

        XMLPortParam(LevelInfo, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(LevelInfo, "gametype", setGametype, getGametype, xmlelement, mode).defaultValues("Gametype");
        XMLPortParam(LevelInfo, "skybox", setSkybox, getSkybox, xmlelement, mode);
        XMLPortParam(LevelInfo, "ambientlight", setAmbientLight, getAmbientLight, xmlelement, mode).defaultValues(ColourValue(0.2, 0.2, 0.2, 1));
    }

    void LevelInfo::registerVariables()
    {
        REGISTERSTRING(name_,         network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::changedName));
        REGISTERSTRING(description_,  network::direction::toclient);
        REGISTERSTRING(skybox_,       network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::applySkybox));
        REGISTERDATA(ambientLight_, network::direction::toclient, new network::NetworkCallback<LevelInfo>(this, &LevelInfo::applyAmbientLight));
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

    void LevelInfo::setGametype(const std::string& gametype)
    {
        Identifier* identifier = ClassByString(gametype);
        if (identifier)
        {
            this->gametype_ = gametype;
            this->gametypeIdentifier_ = identifier;
            this->gametypeInstance_ = this->gametypeIdentifier_.fabricate();
        }
    }
}
