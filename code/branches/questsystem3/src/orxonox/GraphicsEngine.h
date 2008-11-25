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
 *      Reto Grieder
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      Felix Schulthess
 *
 */

/**
  @file
  @brief Declaration of GraphicsEngine Singleton.
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _GraphicsEngine_H__
#define _GraphicsEngine_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include <OgrePrerequisites.h>
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief Graphics engine manager class
    */
    class _OrxonoxExport GraphicsEngine : public OrxonoxClass
    {
        // HACK: temporary means
        friend class GSGraphics;

    public:
        GraphicsEngine();
        ~GraphicsEngine();

        void setConfigValues();
        void detailLevelParticleChanged();

        float getAverageFramesPerSecond() const   { return this->avgFramesPerSecond_; }
        float getAverageTickTime() const          { return this->avgTickTime_; }
        void setAverageTickTime(float tickTime)   { this->avgTickTime_ = tickTime; }
        void setAverageFramesPerSecond(float fps) { this->avgFramesPerSecond_ = fps; }

        inline unsigned int getDetailLevelParticle() const
        { return this->detailLevelParticle_; }

        static GraphicsEngine& getInstance();
        static GraphicsEngine* getInstancePtr() { return singletonRef_s; }

    private:
        // don't mess with singletons
        GraphicsEngine(GraphicsEngine&);

        // stats
        float               avgTickTime_;           //!< time in ms to tick() one frame
        float               avgFramesPerSecond_;    //!< number of frames processed in one second

        // config values
        unsigned int        detailLevelParticle_;   //!< Detail level of particle effects (0: off, 1: low, 2: normal, 3: high)

        static GraphicsEngine* singletonRef_s;      //!< Pointer to the Singleton
    };
}

#endif /* _GraphicsEngine_H__ */
