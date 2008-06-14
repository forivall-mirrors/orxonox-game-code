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
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "OverlayGroup.h"

#include <assert.h>
#include "core/Debug.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "OrxonoxOverlay.h"

namespace orxonox
{
  CreateFactory(OverlayGroup);

  SetConsoleCommand(OverlayGroup, toggleVisibility, false).setAccessLevel(AccessLevel::User);

  OverlayGroup* OverlayGroup::hudInstance_s = 0;

  using namespace Ogre;

  OverlayGroup::OverlayGroup()
    : scale_(1.0, 1.0)
  {
    RegisterObject(OverlayGroup);

    // Singleton like in Ogre. Constructor and destructor are public,
    // but the assert prevents from having multiple instances.
    assert(hudInstance_s == 0);
    hudInstance_s = this;
  }

  OverlayGroup::~OverlayGroup()
  {
    if (this->isInitialized())
    {
    }

    hudInstance_s = 0;
  }

  void OverlayGroup::XMLPort(Element& xmlElement, XMLPort::Mode mode)
  {
    BaseObject::XMLPort(xmlElement, mode);

    XMLPortObject(OverlayGroup, OrxonoxOverlay, "", addElement, getElement, xmlElement, mode, false, true);
    XMLPortParam(OverlayGroup, "scale", scale, getScale, xmlElement, mode);
  }

  void OverlayGroup::scale(const Vector2& scale)
  {
    for (std::map<std::string, OrxonoxOverlay*>::iterator it = hudElements_.begin(); it != hudElements_.end(); ++it)
      (*it).second->scale(scale);
    this->scale_ = scale;
  }

  void OverlayGroup::addElement(OrxonoxOverlay* element)
  {
    if (hudElements_.find(element->getName()) != hudElements_.end())
    {
      COUT(1) << "Ambiguous names encountered while load the HUD overlays" << std::endl;
    }
    else
      hudElements_[element->getName()] = element;
  }

  OrxonoxOverlay* OverlayGroup::getElement(unsigned int index)
  {
    if (index < this->hudElements_.size())
    {
      std::map<std::string, OrxonoxOverlay*>::const_iterator it = hudElements_.begin();
      for (unsigned int i = 0; i != index; ++it, ++i)
        ;
      return (*it).second;
    }
    else
      return 0;
  }

  /*static*/ OverlayGroup& OverlayGroup::getHUD()
  {
    assert(hudInstance_s);
    return *hudInstance_s;
  }

  /*static*/ void OverlayGroup::toggleVisibility(const std::string& name)
  {
    if (OverlayGroup::getHUD().hudElements_.find(name) != OverlayGroup::getHUD().hudElements_.end())
    {
      OverlayGroup::getHUD().hudElements_[name]->setVisibility(!OverlayGroup::getHUD().hudElements_[name]->isVisible());
    }
  }

}
