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

/* Folgender Block ist Copy-Paste und somit teilweise wohl unnoetig */
#include "core/Template.h"
#include "core/XMLPort.h"
#include "gametypes/Gametype.h"
#include "worldentities/pawns/Pawn.h"

/* Eigene, spezifische include-Statements*/
#include "worldentities/MobileEntity.h"
#include "worldentities/ControllableEntity.h"
#include "core/ObjectListIterator.h"
#include <worldentities/pawns/Pawn.h>
#include <infos/PlayerInfo.h>

#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayManager.h>
#include <overlays/OverlayText.h>

namespace orxonox
{
    CreateFactory(SpaceBoundaries);

    SpaceBoundaries::SpaceBoundaries(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(SpaceBoundaries);
        COUT(0) << "Test ob Konstruktor aufgerufen wird." << std::endl; //!< message for debugging
        // Show Boundaries on the radar.
         m_pColoredTextAreaOverlayElementFactory = new ColoredTextAreaOverlayElementFactory();
    }
    SpaceBoundaries::~SpaceBoundaries()
    {
        delete pColoredTextAreaOverlayElementFactory;
    }
    
    void SpaceBoundaries::setCenter(Vector3 r)
    {
        this->center = r;
    }
    Vector3 SpaceBoundaries::getCenter()
    {
        return this->center;
    }
    
    void SpaceBoundaries::setMaxDistance(float r)
    {
        this->maxDistance = r;
    }
    float SpaceBoundaries::getMaxDistance()
    {
        return this->maxDistance;
    }
    
    void SpaceBoundaries::setWarnDistance(float r)
    {
        this->warnDistance = r;
    }
    float SpaceBoundaries::getWarnDistance()
    {
        return this->warnDistance;
    }

    void SpaceBoundaries::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceBoundaries, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceBoundaries, "center", setCenter, getCenter, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "maxDistance", setMaxDistance, getMaxDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "warnDistance", setWarnDistance, getWarnDistance, xmlelement, mode);
    }
    
    void SpaceBoundaries::tick(float dt)
    {
        for(ObjectListIterator<MobileEntity> item = ObjectList<MobileEntity>::begin(); item != ObjectList<MobileEntity>::end(); ++item)
        {
            MobileEntity* myMobileEntity = *item;
            Pawn* myItem = dynamic_cast<Pawn*>(myMobileEntity);
            //COUT(0) << "Die for-Schleife wird erreicht!!!" << std::endl; //!< message for debugging
            if(myItem != NULL)
            {
                float distance = computeDistance((WorldEntity*) myItem);
                bool humanItem = this->isHumanPlayer(myItem);
                COUT(0) << "Pawn wird erkannt!!!" << std::endl; //!< message for debugging
                COUT(0) << "Distanz:" << distance << std::endl; //!< message for debugging
                if(distance > this->warnDistance /*&& distance < this->maxDistance*/)
                {
                    COUT(0) << "You are leaving the area" << std::endl; //!< message for debugging
                    if(humanItem)
                    {
                        COUT(0) << "humanItem ist true" << std::endl;
                        this->displayWarning("Attention! You are leaving the area!");
                    } else {
                    
                    }
                } else if(distance > maxDistance)
                {
                    // Decrease Health
                    if(humanItem)
                    {
                        
                    } else {
                    
                    }
                }
            }
        }
    }
    
    float SpaceBoundaries::computeDistance(WorldEntity *item)
    {
        Vector3 itemPosition = item->getPosition();
        return (itemPosition.distance(this->center));
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
         OverlayManager::getSingleton().addOverlayElementFactory(pColoredTextAreaOverlayElementFactory);
         
        Ogre::TextAreaOverlayElement *pTextArea =
                (Ogre::TextAreaOverlayElement*)Ogre::OverlayManager.createOverlayElement("TextArea", "MyTextArea");
        pTextArea->setCaption("Some plain text");

        
    }
    
    bool SpaceBoundaries::isHumanPlayer(Pawn *item)
    {
        if(item != NULL)
        {
            return item->getPlayer()->isHumanPlayer();
        } else {
            return false;
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
