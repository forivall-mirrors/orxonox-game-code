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
 *
 */

#ifndef _HUDNavigation_H__
#define _HUDNavigation_H__

#include "overlays/OverlaysPrereqs.h"

#include <map>
#include <string>


#include "util/OgreForwardRefs.h"
#include "tools/interfaces/Tickable.h"
#include "interfaces/RadarListener.h"
#include "overlays/OrxonoxOverlay.h"

namespace orxonox
{
class _OverlaysExport HUDNavigation : public OrxonoxOverlay, public Tickable, public RadarListener
{
public:
    HUDNavigation ( BaseObject* creator );
    virtual ~HUDNavigation();

    void setConfigValues();

    virtual void XMLPort ( Element& xmlelement, XMLPort::Mode mode );
    virtual void tick ( float dt );

    virtual void addObject ( RadarViewable* object );
    virtual void removeObject ( RadarViewable* viewable );
    virtual void objectChanged ( RadarViewable* viewable );

    virtual void changedOwner();
    virtual void sizeChanged();
    virtual void angleChanged() { }
    virtual void positionChanged() { }
    virtual void radarTick ( float dt ) {}

    inline float getRadarSensitivity() const
    { return 1.0f; }

private:
    struct ObjectInfo
    {
        Ogre::PanelOverlayElement* panel_;
        Ogre::TextAreaOverlayElement* text_;
        bool outOfView_;
        bool wasOutOfView_;

    };

    bool showObject( RadarViewable* rv );

    // XMLPort accessors
    void setNavMarkerSize ( float size )
    { navMarkerSize_ = size; this->sizeChanged(); }
    float getNavMarkerSize() const
    { return navMarkerSize_; }

    void setDetectionLimit( float limit )
    { this->detectionLimit_ = limit; }
    float getDetectionLimit() const
    { return this->detectionLimit_; }

    void setTextSize ( float size );
    float getTextSize() const;

    void setFont ( const std::string& font );
    const std::string& getFont() const;

    typedef std::map<RadarViewable*, ObjectInfo > ObjectMap;
    ObjectMap activeObjectList_;

    typedef std::list < std::pair<RadarViewable*, unsigned int > > sortedList;
    sortedList sortedObjectList_;


    float navMarkerSize_;
    std::string fontName_;
    float textSize_;

    unsigned int markerLimit_; //TODO: is it possible to set this over the console and/or the IG-Setting
    float detectionLimit_; //!< Objects that are more far away than detectionLimit_ are not displayed on the HUD. 10000.0f is the default value.
                           //!< In order to bypass this behaviour, set a negative detectionLimit_. Then the detection range is "infinite".
};
}

#endif /* _HUDNavigation_H__ */
