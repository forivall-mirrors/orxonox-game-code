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
 *      Maurus Kaufmann
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceBoundaries.h"

#include "worldentities/MobileEntity.h"
#include "worldentities/ControllableEntity.h"
#include "core/ObjectListIterator.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
#include "infos/PlayerInfo.h"
#include "interfaces/RadarViewable.h"


//#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayManager.h>
//#include <overlays/OverlayText.h>

namespace orxonox
{
    CreateFactory(SpaceBoundaries);

    SpaceBoundaries::SpaceBoundaries(BaseObject* creator) : StaticEntity(creator)
    {
        /* Standardwerte, die zum Tragen kommen,
         * falls im XML-File keine Werte spezifiziert wurden. */
        this->setMaxDistance(3000);
        this->setWarnDistance(2000);
        this->setHealthDecrease(1);
        
        RegisterObject(SpaceBoundaries);
        
        // Show Boundaries on the radar.
        this->centerRadar_ = new RadarViewable(this, this);
        this->centerRadar_->setRadarObjectShape(RadarViewable::Dot);
        this->centerRadar_->setRadarVisibility(false);
        
//         m_pColoredTextAreaOverlayElementFactory = new ColoredTextAreaOverlayElementFactory();
    }
    SpaceBoundaries::~SpaceBoundaries()
    {
        delete this->centerRadar_;
//        delete pColoredTextAreaOverlayElementFactory;
    }
    
    void SpaceBoundaries::setMaxDistance(float r)
    {
        this->maxDistance_ = r;
    }
    float SpaceBoundaries::getMaxDistance()
    {
        return this->maxDistance_;
    }
    
    void SpaceBoundaries::setWarnDistance(float r)
    {
        this->warnDistance_ = r;
    }
    float SpaceBoundaries::getWarnDistance()
    {
        return this->warnDistance_;
    }
    
    void SpaceBoundaries::setHealthDecrease(float amount)
    {
        this->healthDecrease_ = amount/1000;
    }
    float SpaceBoundaries::getHealthDecrease()
    {
        return this->healthDecrease_;
    }

    void SpaceBoundaries::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceBoundaries, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceBoundaries, "maxDistance", setMaxDistance, getMaxDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "warnDistance", setWarnDistance, getWarnDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "healthDecrease", setHealthDecrease, getHealthDecrease, xmlelement, mode);
    }
    
    void SpaceBoundaries::tick(float dt)
    {
        for(ObjectListIterator<MobileEntity> item = ObjectList<MobileEntity>::begin(); item != ObjectList<MobileEntity>::end(); ++item)
        {
            MobileEntity* myMobileEntity = *item;
            Pawn* myItem = dynamic_cast<Pawn*>(myMobileEntity);
            if(myItem != NULL)
            {
                float distance = computeDistance((WorldEntity*) myItem);
                bool humanItem = this->isHumanPlayer(myItem);
                COUT(0) << "Distanz:" << distance << std::endl; //!< message for debugging
                if(distance > this->warnDistance_ && distance < this->maxDistance_)
                {
                    COUT(0) << "You are leaving the area" << std::endl; //!< message for debugging
                    if(humanItem)
                    {
                        COUT(0) << "humanItem ist true" << std::endl;
                        this->displayWarning("Attention! You are leaving the area!");
                    } else {
                        
                    }
                }
                if(distance > maxDistance_)
                {
                    if(humanItem)
                    {
                        COUT(0) << "Health should be decreasing!" << std::endl;
                        this->displayWarning("You are out of the area now!");
                        myItem->removeHealth( (distance - maxDistance_) * this->healthDecrease_);
                    } else {
                        
                    }
                }
            }
        }
    }
    
    float SpaceBoundaries::computeDistance(WorldEntity *item)
    {
        Vector3 itemPosition = item->getPosition();
        return (itemPosition.distance(this->getPosition()));
    }
    
    void SpaceBoundaries::displayWarning(const std::string warnText)
    {   /*
        Ogre::TextAreaOverlayElement *pTextArea;
        Ogre::OverlayManager manager = Ogre::OverlayManager();
        Ogre::OverlayElement temp = manager.createOverlayElement("TextArea", "MyTextArea");
        //pTextArea = (Ogre::TextAreaOverlayElement *) 
       // pTextArea->setCaption("Some plain text");
    
        Ogre::TextAreaOverlayElement warning(warnText);
        warning.initialise();
        //warning.setPosition(0.2, 0.2);
        

        COUT(0) << "Breite des Warntextes: " << warning.getWidth() << std::endl;
        COUT(0) << "Hoehe des Warntextes: " << warning.getHeight() << std::endl;
        
        warning.show();*/
        // Register the overlay element
/*         OverlayManager::getSingleton().addOverlayElementFactory(pColoredTextAreaOverlayElementFactory);
         
        Ogre::TextAreaOverlayElement *pTextArea =
                (Ogre::TextAreaOverlayElement*)Ogre::OverlayManager.createOverlayElement("TextArea", "MyTextArea");
        pTextArea->setCaption("Some plain text");
*/
        
    }
    
    bool SpaceBoundaries::isHumanPlayer(Pawn *item)
    {
        if(item != NULL)
        {
            if(item->getPlayer())
            {
                return item->getPlayer()->isHumanPlayer();
            }
        }
        return false;
    }
    
}
