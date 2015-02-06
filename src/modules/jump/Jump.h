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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Jump_H__
#define _Jump_H__

#include "jump/JumpPrereqs.h"
#include "tools/Timer.h"
#include "graphics/Camera.h"
#include "gametypes/Deathmatch.h"
#include "JumpCenterpoint.h"
#include <list>

namespace orxonox
{
    class _JumpExport Jump : public Deathmatch
    {
        public:
            Jump(Context* context);
            virtual ~Jump();
            virtual void tick(float dt);
            virtual void start();
            virtual void end();
            virtual void spawnPlayer(PlayerInfo* player);
            int getScore(PlayerInfo* player) const;
            float getFuel() const;
            bool getDead(PlayerInfo* player) const;
            void setCenterpoint(JumpCenterpoint* center)
                { center_ = center; }
            PlayerInfo* getPlayer() const;

        protected:
            void cleanup();
            virtual void addPlatform(JumpPlatform* newPlatform, std::string platformTemplate, float xPosition, float zPosition);
            virtual JumpPlatformStatic* addPlatformStatic(float xPosition, float zPosition);
            virtual JumpPlatformHMove* addPlatformHMove(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float speed);
            virtual JumpPlatformVMove* addPlatformVMove(float xPosition, float zPosition, float lowerBoundary, float upperBoundary, float speed);
            virtual JumpPlatformDisappear* addPlatformDisappear(float xPosition, float zPosition);
            virtual JumpPlatformTimer* addPlatformTimer(float xPosition, float zPosition, float time, float variance);
            virtual JumpPlatformFake* addPlatformFake(float xPosition, float zPosition);
            virtual void addProjectile(float xPosition, float zPosition);
            virtual void addEnemy(int type, float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addSpring(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addSpring(JumpPlatform* platform);
            virtual void addRocket(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addRocket(JumpPlatform* platform);
            virtual void addPropeller(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addPropeller(JumpPlatform* platform);
            virtual void addBoots(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addBoots(JumpPlatform* platform);
            virtual void addShield(float xPosition, float zPosition, float leftBoundary, float rightBoundary, float lowerBoundary, float upperBoundary, float xVelocity, float zVelocity);
            virtual void addShield(JumpPlatform* platform);
            virtual void addStartSection();
            virtual void addSection();
            virtual bool addAdventure(int number);
            virtual float randomXPosition();
            virtual float randomXPosition(int totalColumns, int culomn);
            virtual float randomXPositionLeft(int totalColumns, int culomn);
            virtual float randomXPositionRight(int totalColumns, int culomn);
            virtual float randomZPosition(int totalRows, int row, float sectionBegin, float SectionEnd);
            virtual float randomZPositionLower(int totalRows, int row, float sectionBegin, float SectionEnd);
            virtual float randomZPositionUpper(int totalRows, int row, float sectionBegin, float SectionEnd);
            virtual float randomPosition(float lowerBoundary, float upperBoundary);
            virtual float randomSpeed();

            WeakPtr<JumpCenterpoint> center_;
            WeakPtr<JumpFigure> figure_;
            WeakPtr<Camera> camera;
            float totalScreenShift_;
            float screenShiftSinceLastUpdate_;
            int sectionNumber_;
            int adventureNumber_;
            float platformWidth_;
            float platformHeight_;
    };
}

#endif /* _Jump_H__ */
