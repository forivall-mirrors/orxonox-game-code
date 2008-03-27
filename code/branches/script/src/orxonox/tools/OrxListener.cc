/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include "core/CoreIncludes.h"
#include "objects/NPC.h"
#include "audio/AudioManager.h"
#include "OrxListener.h"

namespace orxonox
{
  OrxListener::OrxListener(audio::AudioManager*  auMan, gameMode mode)
  {
    mode_=mode;
    auMan_ = auMan;
  }

  void OrxListener::tick(float dt)
  {
    auMan_->update();
    updateAI();
  }

  void OrxListener::updateAI()
  {
    for(Iterator<NPC> it = ObjectList<NPC>::start(); it; ++it)
    {
      it->update();
    }
  }
}
