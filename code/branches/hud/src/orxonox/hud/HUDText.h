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

#ifndef _HUDText_H__
#define _HUDText_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "util/Math.h"
#include "HUDOverlay.h"

namespace orxonox
{
  class _OrxonoxExport HUDText : public HUDOverlay
  {
  public:
    HUDText();
    virtual ~HUDText();

    virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

  protected:
    void setBackgroundMaterial(const std::string& material);
    std::string getBackgroundMaterial() const;
    void setCaption(const std::string& caption);
    const std::string& getCaption() const;
    void setFont(const std::string& font);
    std::string getFont() const;

    Ogre::TextAreaOverlayElement* text_;
  private:
    std::string caption_;
    Ogre::PanelOverlayElement* background_;
  };
}
#endif /* _HUDText_H__ */
