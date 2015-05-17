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

/**
    @file JumpPlatformTimer.h
    @brief Declaration of the JumpPlatformTimer class.
    @ingroup Jump
*/

#ifndef _JumpPlatformTimer_H__
#define _JumpPlatformTimer_H__

#include "JumpPrereqs.h"
#include "JumpPlatform.h"

namespace orxonox
{
    class _JumpExport JumpPlatformTimer : public JumpPlatform
    {
        public:
            JumpPlatformTimer(Context* context);
            virtual ~JumpPlatformTimer();
            virtual void tick(float dt);
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void setProperties(float time);
            virtual bool isActive(void);
            virtual void touchFigure();

            void setEffectPath(const std::string& effectPath)
                { this->effectPath_ = effectPath; }

            const std::string& getEffectPath() const
                { return this->effectPath_; }

            void setEffectStartTime(const float effectStartTime)
                { this->effectStartTime_ = effectStartTime; }

            const float getEffectStartTime() const
                { return this->effectStartTime_; }

        protected:
            float time_;
            ParticleSpawner* particleSpawner_;
            std::string effectPath_;
            float effectStartTime_;
    };
}

#endif /* _JumpPlatformTimer_H__ */
