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
 *      Gion-Andri Cantieni
 *   Co-authors:
 *      ...
 *
 */


#include "core/OrxonoxClass.h"
#include "util/Singleton.h"
#include "tools/interfaces/Tickable.h"
#include <OgreMath.h>


namespace orxonox
{
    class SkyboxGenerator : public virtual OrxonoxClass, public Singleton<SkyboxGenerator>, public Tickable
    {
        friend class Singleton<SkyboxGenerator>;

        public:
            SkyboxGenerator();
            virtual ~SkyboxGenerator();
            static void createSkybox( );
            void setConfigValues( );
            void tick(float dt);

        private:
            static SkyboxGenerator* singletonPtr_s;
            std::string skyboxPrefix_;
            bool takeScreenshot_;
            int iterateOverDirections_;
            float aspectRatio_;
            Ogre::Radian fovy_;
            bool captionsRemoved_;
    };
}
