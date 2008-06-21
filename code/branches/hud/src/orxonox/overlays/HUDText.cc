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
#include "HUDText.h"

#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "util/Convert.h"

namespace orxonox
{
  CreateFactory(HUDText);

  using namespace Ogre;

  HUDText::HUDText()
    : text_(0)
  {
    RegisterObject(HUDText);
  }

  HUDText::~HUDText()
  {
    if (this->isInitialized())
    {
      if (this->text_)
          OverlayManager::getSingleton().destroyOverlayElement(this->text_);
    }
  }

  void HUDText::XMLPort(Element& xmlElement, XMLPort::Mode mode)
  {
    OrxonoxOverlay::XMLPort(xmlElement, mode);

    if (mode == XMLPort::LoadObject)
    {
      this->text_ = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", getName() + "_Text"));
      this->text_->setCharHeight(1.0f);
      this->text_->setFontName("Monofur");

      this->background_->addChild(this->text_);
    }

    XMLPortParam(HUDText, "font", setFont, getFont, xmlElement, mode);
    XMLPortParam(HUDText, "caption", setCaption, getCaption, xmlElement, mode);

    if (mode == XMLPort::LoadObject)
    {
      this->text_->setCaption(this->caption_);
    }
  }

  void HUDText::setCaption(const std::string& caption)
  {
    this->caption_ = caption;
  }

  const std::string& HUDText::getCaption() const
  {
    return this->caption_;
  }

  void HUDText::setFont(const std::string& font)
  {
    if (this->text_ && font != "")
      this->text_->setFontName(font);
  }

  std::string HUDText::getFont() const
  {
    if (this->text_)
      return this->text_->getFontName();
    else
      return "";
  }
}
