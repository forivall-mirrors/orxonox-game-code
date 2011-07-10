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
 *      Felix Schulthess
 *   Co-authors:
 *      Reto Grieder
 *      Oliver Scheuss
 *
 */

#include "HUDNavigation.h"

#include <OgreCamera.h>
#include <OgreFontManager.h>
#include <OgreOverlayManager.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include "util/Math.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "CameraManager.h"
#include "Scene.h"
#include "Radar.h"
#include "graphics/Camera.h"
#include "controllers/HumanController.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/WorldEntity.h"
#include "core/ConfigValueIncludes.h"
#include "tools/TextureGenerator.h"
// #include <boost/bind/bind_template.hpp>


namespace orxonox
{
bool compareDistance ( std::pair<RadarViewable*, unsigned int > a, std::pair<RadarViewable*, unsigned int > b )
{
    return a.second<b.second;

}

void HUDNavigation::setConfigValues()
{
  SetConfigValue(markerLimit_, 3);
}

CreateFactory ( HUDNavigation );

HUDNavigation::HUDNavigation ( BaseObject* creator )
        : OrxonoxOverlay ( creator )
{
    RegisterObject ( HUDNavigation );
    this->setConfigValues();

    // Set default values
    setFont ( "Monofur" );
    setTextSize ( 0.05f );
    setNavMarkerSize ( 0.05f );
}

HUDNavigation::~HUDNavigation()
{
    if ( this->isInitialized() )
    {
        for ( ObjectMap::iterator it = activeObjectList_.begin(); it != activeObjectList_.end(); )
            removeObject ( ( it++ )->first );

    }

    sortedObjectList_.clear();
}

void HUDNavigation::XMLPort ( Element& xmlelement, XMLPort::Mode mode )
{
    SUPER ( HUDNavigation, XMLPort, xmlelement, mode );

    XMLPortParam ( HUDNavigation, "font",          setFont,          getFont,          xmlelement, mode );
    XMLPortParam ( HUDNavigation, "textSize",      setTextSize,      getTextSize,      xmlelement, mode );
    XMLPortParam ( HUDNavigation, "navMarkerSize", setNavMarkerSize, getNavMarkerSize, xmlelement, mode );
}

void HUDNavigation::setFont ( const std::string& font )
{
    const Ogre::ResourcePtr& fontPtr = Ogre::FontManager::getSingleton().getByName ( font );
    if ( fontPtr.isNull() )
    {
        COUT ( 2 ) << "Warning: HUDNavigation: Font '" << font << "' not found" << std::endl;
        return;
    }
    fontName_ = font;
    for ( ObjectMap::iterator it = activeObjectList_.begin(); it != activeObjectList_.end(); ++it )
    {
        if ( it->second.text_ != NULL )
            it->second.text_->setFontName ( fontName_ );
    }
}

const std::string& HUDNavigation::getFont() const
{
    return fontName_;
}

void HUDNavigation::setTextSize ( float size )
{
    if ( size <= 0.0f )
    {
        COUT ( 2 ) << "Warning: HUDNavigation: Negative font size not allowed" << std::endl;
        return;
    }
    textSize_ = size;
    for ( ObjectMap::iterator it = activeObjectList_.begin(); it!=activeObjectList_.end(); ++it )
    {
        if ( it->second.text_ )
            it->second.text_->setCharHeight ( size );
    }
}

float HUDNavigation::getTextSize() const
{
    return textSize_;
}



void HUDNavigation::tick ( float dt )
{
    SUPER ( HUDNavigation, tick, dt );

    Camera* cam = CameraManager::getInstance().getActiveCamera();
    if ( cam == NULL )
        return;
    const Matrix4& camTransform = cam->getOgreCamera()->getProjectionMatrix() * cam->getOgreCamera()->getViewMatrix();


    for ( sortedList::iterator listIt = sortedObjectList_.begin(); listIt != sortedObjectList_.end(); ++listIt )
    {
        listIt->second = ( int ) ( ( listIt->first->getRVWorldPosition() - HumanController::getLocalControllerSingleton()->getControllableEntity()->getWorldPosition() ).length() + 0.5f );
    }

    sortedObjectList_.sort ( compareDistance );

    unsigned int markerCount_ = 0;

//         for (ObjectMap::iterator it = activeObjectList_.begin(); it != activeObjectList_.end(); ++it)
    for ( sortedList::iterator listIt = sortedObjectList_.begin(); listIt != sortedObjectList_.end(); ++markerCount_, ++listIt )
    {
        ObjectMap::iterator it = activeObjectList_.find ( listIt->first );

        if ( markerCount_ < markerLimit_ )
        {


            // Get Distance to HumanController and save it in the TextAreaOverlayElement.
            int dist = listIt->second;
            it->second.text_->setCaption ( multi_cast<std::string> ( dist ) );
            float textLength = multi_cast<std::string> ( dist ).size() * it->second.text_->getCharHeight() * 0.3f;

            // Transform to screen coordinates
            Vector3 pos = camTransform * it->first->getRVWorldPosition();

            bool outOfView = true;
            if ( pos.z > 1.0 )
            {
                // z > 1.0 means that the object is behind the camera
                outOfView = true;
                // we have to switch all coordinates (if you don't know why,
                // try linear algebra lectures, because I can't explain..)
                pos.x = -pos.x;
                pos.y = -pos.y;
            }
            else
                outOfView = pos.x < -1.0 || pos.x > 1.0 || pos.y < -1.0 || pos.y > 1.0;
            // Get Distance to HumanController and save it in the TextAreaOverlayElement.
            it->second.text_->setCaption ( multi_cast<std::string> ( dist ) );

            if ( outOfView )
            {
                // Object is not in view

                // Change material only if outOfView changed
                if ( !it->second.wasOutOfView_ )
                {
                    it->second.panel_->setMaterialName( TextureGenerator::getMaterialName( "arrows.png", it->first->getRadarObjectColour()) );
                    it->second.wasOutOfView_ = true;
                }

                // Switch between top, bottom, left and right position of the arrow at the screen border
                if ( pos.x < pos.y )
                {
                    if ( pos.y > -pos.x )
                    {
                        // Top
                        float position = pos.x / pos.y + 1.0f;
                        it->second.panel_->setPosition ( ( position - it->second.panel_->getWidth() ) * 0.5f, 0.0f );
                        it->second.panel_->setUV ( 0.5f, 0.0f, 1.0f, 0.5f );
                        it->second.text_->setLeft ( ( position - textLength ) * 0.5f );
                        it->second.text_->setTop ( it->second.panel_->getHeight() );
                    }
                    else
                    {
                        // Left
                        float position = pos.y / pos.x + 1.0f;
                        it->second.panel_->setPosition ( 0.0f, ( position - it->second.panel_->getWidth() ) * 0.5f );
                        it->second.panel_->setUV ( 0.0f, 0.0f, 0.5f, 0.5f );
                        it->second.text_->setLeft ( it->second.panel_->getWidth() + 0.01f );
                        it->second.text_->setTop ( ( position - it->second.text_->getCharHeight() ) * 0.5f );
                    }
                }

                else
                {

                    if ( pos.y < -pos.x )
                    {
                        // Bottom
                        float position = -pos.x / pos.y + 1.0f;
                        it->second.panel_->setPosition ( ( position - it->second.panel_->getWidth() ) * 0.5f, 1.0f - it->second.panel_->getHeight() );
                        it->second.panel_->setUV ( 0.0f, 0.5f, 0.5f, 1.0f );
                        it->second.text_->setLeft ( ( position - textLength ) * 0.5f );
                        it->second.text_->setTop ( 1.0f - it->second.panel_->getHeight() - it->second.text_->getCharHeight() );
                    }
                    else
                    {
                        // Right
                        float position = -pos.y / pos.x + 1.0f;
                        it->second.panel_->setPosition ( 1.0f - it->second.panel_->getWidth(), ( position - it->second.panel_->getHeight() ) * 0.5f );
                        it->second.panel_->setUV ( 0.5f, 0.5f, 1.0f, 1.0f );
                        it->second.text_->setLeft ( 1.0f - it->second.panel_->getWidth() - textLength - 0.01f );
                        it->second.text_->setTop ( ( position - it->second.text_->getCharHeight() ) * 0.5f );
                    }
                }
            }
            else
            {
                // Object is in view

                // Change material only if outOfView changed
                if ( it->second.wasOutOfView_ )
                {
                  //it->second.panel_->setMaterialName ( "Orxonox/NavTDC" );
                    it->second.panel_->setMaterialName( TextureGenerator::getMaterialName( "tdc.png", it->first->getRadarObjectColour()) );
                    it->second.wasOutOfView_ = false;
                }

                // Position marker
                it->second.panel_->setUV ( 0.0f, 0.0f, 1.0f, 1.0f );
                it->second.panel_->setLeft ( ( pos.x + 1.0f - it->second.panel_->getWidth() ) * 0.5f );
                it->second.panel_->setTop ( ( -pos.y + 1.0f - it->second.panel_->getHeight() ) * 0.5f );

                // Position text
                it->second.text_->setLeft ( ( pos.x + 1.0f + it->second.panel_->getWidth() ) * 0.5f );
                it->second.text_->setTop ( ( -pos.y + 1.0f + it->second.panel_->getHeight() ) * 0.5f );
            }

            // Make sure the overlays are shown
            it->second.panel_->show();
            it->second.text_->show();
        }
        else
        {
            it->second.panel_->hide();
            it->second.text_->hide();
        }

    }
}


/** Overridden method of OrxonoxOverlay.
@details
    Usually the entire overlay scales with scale().
    Here we obviously have to adjust this.
*/
void HUDNavigation::sizeChanged()
{
    // Use size to compensate for aspect ratio if enabled.
    float xScale = this->getActualSize().x;
    float yScale = this->getActualSize().y;

    for ( ObjectMap::iterator it = activeObjectList_.begin(); it!=activeObjectList_.end(); ++it )
    {
        if ( it->second.panel_ != NULL )
            it->second.panel_->setDimensions ( navMarkerSize_ * xScale, navMarkerSize_ * yScale );
        if ( it->second.text_ != NULL )
            it->second.text_->setCharHeight ( it->second.text_->getCharHeight() * yScale );
    }
}

void HUDNavigation::addObject ( RadarViewable* object )
{
    if( showObject(object)==false )
        return;

    if ( activeObjectList_.size() >= markerLimit_ )
        if ( object == NULL )
            return;

    // Object hasn't been added yet (we know that)
    assert ( this->activeObjectList_.find ( object ) == this->activeObjectList_.end() );

    // Scales used for dimensions and text size
    float xScale = this->getActualSize().x;
    float yScale = this->getActualSize().y;

    // Create everything needed to display the object on the radar and add it to the map

    // Create arrow/marker
    Ogre::PanelOverlayElement* panel = static_cast<Ogre::PanelOverlayElement*> ( Ogre::OverlayManager::getSingleton()
                                       .createOverlayElement ( "Panel", "HUDNavigation_navMarker_" + getUniqueNumberString() ) );
//     panel->setMaterialName ( "Orxonox/NavTDC" );
    panel->setMaterialName( TextureGenerator::getMaterialName( "tdc.png", object->getRadarObjectColour()) );
    panel->setDimensions ( navMarkerSize_ * xScale, navMarkerSize_ * yScale );
//     panel->setColour( object->getRadarObjectColour() );

    Ogre::TextAreaOverlayElement* text = static_cast<Ogre::TextAreaOverlayElement*> ( Ogre::OverlayManager::getSingleton()
                                         .createOverlayElement ( "TextArea", "HUDNavigation_navText_" + getUniqueNumberString() ) );
    text->setFontName ( this->fontName_ );
    text->setCharHeight ( text->getCharHeight() * yScale );
    text->setColour( object->getRadarObjectColour() );

    panel->hide();
    text->hide();

    ObjectInfo tempStruct = {panel, text, false /*, TODO: initialize wasOutOfView_ */};
    activeObjectList_[object] = tempStruct;

    this->background_->addChild ( panel );
    this->background_->addChild ( text );

    sortedObjectList_.push_front ( std::make_pair ( object, ( unsigned int ) 0 ) );


}

void HUDNavigation::removeObject ( RadarViewable* viewable )
{
    ObjectMap::iterator it = activeObjectList_.find ( viewable );

    if ( activeObjectList_.find ( viewable ) != activeObjectList_.end() )
    {
        // Detach overlays
        this->background_->removeChild ( it->second.panel_->getName() );
        this->background_->removeChild ( it->second.text_->getName() );
        // Properly destroy the overlay elements (do not use delete!)
        Ogre::OverlayManager::getSingleton().destroyOverlayElement ( it->second.panel_ );
        Ogre::OverlayManager::getSingleton().destroyOverlayElement ( it->second.text_ );
        // Remove from the list
        activeObjectList_.erase ( viewable );


    }

    for ( sortedList::iterator listIt = sortedObjectList_.begin(); listIt != sortedObjectList_.end(); ++listIt )
    {
        if ( (listIt->first) == viewable )
        {
            sortedObjectList_.erase ( listIt );
            break;
        }

    }

}

void HUDNavigation::objectChanged(RadarViewable* viewable)
{
    // TODO: niceification neccessary ;)
    removeObject(viewable);
    addObject(viewable);
}


bool HUDNavigation::showObject(RadarViewable* rv)
{
    if ( rv == dynamic_cast<RadarViewable*> ( this->getOwner() ) )
        return false;
    assert( rv->getWorldEntity() );
    if ( rv->getWorldEntity()->isVisible()==false || rv->getRadarVisibility()==false )
        return false;
    return true;
}

void HUDNavigation::changedOwner()
{

    const std::set<RadarViewable*>& respawnObjects = this->getOwner()->getScene()->getRadar()->getRadarObjects();
    for ( std::set<RadarViewable*>::const_iterator it = respawnObjects.begin(); it != respawnObjects.end(); ++it )
    {
        if ( ! ( *it )->isHumanShip_ )
            this->addObject ( *it );
    }
}

}
