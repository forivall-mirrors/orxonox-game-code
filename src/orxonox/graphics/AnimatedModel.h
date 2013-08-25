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
 *      Benjamin de Capitani
 *   Co-authors:
 *      ...
 *
 */

#ifndef _AnimatedModel_H__
#define _AnimatedModel_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/interfaces/Tickable.h"
#include "Model.h"

namespace orxonox
{
    class _OrxonoxExport AnimatedModel : public Model, public Tickable
    {
        public:
            AnimatedModel(Context* context);
            virtual ~AnimatedModel();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            void registerVariables();

            virtual void changedAnimationState();
            virtual void setAnimationState(bool loop, bool enabled, const std::string& state);
            virtual void setAnimEnabled(bool enabled);
            virtual void setAnimLoop(bool loop);
            virtual void tick(float dt);
            virtual void changedMesh();


            inline void setAnims(const std::string& anims)
                { this->anims_ = anims; this->changedAnimationState(); }
            inline const std::string& getAnims() const
                { return this->anims_; }

            inline const std::string& getAnimationState() const
                { return this->anims_; }
            inline const bool&  getAnimLoop() const
                { return this->bAnimLoop_; }
            inline const bool&  getAnimEnabled() const
                { return this->bAnimEnabled_; }

        private:
            std::string anims_;
            bool bAnimEnabled_;
            bool bAnimLoop_;
    };
}

#endif /* _AnimatedModel_H__ */
