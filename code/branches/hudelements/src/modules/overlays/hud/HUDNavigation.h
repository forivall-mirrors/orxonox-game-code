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

#include <utility>
#include <map>
#include "overlays/OverlaysPrereqs.h"
#include "interfaces/RadarViewable.h"

#include "interfaces/RadarListener.h"
#include "util/OgreForwardRefs.h"
#include "tools/interfaces/Tickable.h"
#include "overlays/OrxonoxOverlay.h"

#include <vector>

namespace orxonox
{
class _OverlaysExport HUDNavigation : public OrxonoxOverlay, public Tickable, public RadarListener
{
public:
    HUDNavigation(BaseObject* creator);
    virtual ~HUDNavigation();

    virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);
    virtual void tick(float dt);
    virtual void addObject(RadarViewable* object);

    virtual void removeObject(RadarViewable* viewable);
    virtual void objectChanged(RadarViewable* viewable) {}
    inline float getRadarSensitivity() const {return 1.0f;}
    inline void radarTick(float dt) {}
    virtual void changedOwner();


private:



    void sizeChanged();
    void angleChanged() { }
    void positionChanged() { }

// 	void updateActiveObjectList(map activeObjectList_);



    // XMLPort accessors
    void setNavMarkerSize(float size) {
        navMarkerSize_ = size;
        this->sizeChanged();
    }
    float getNavMarkerSize() const    {
        return navMarkerSize_;
    }

    /*
            void setAimMarkerSize(float size) { this->aimMarkerSize_ = size; this->sizeChanged(); }
            float getAimMarkerSize() const    { return this->aimMarkerSize_; }
    */

    void setTextSize(float size);
    float getTextSize() const;

    void setFont(const std::string& font);
    const std::string& getFont() const;

    void updateMarker();
    void updateFocus();
//     float getDist2Focus() const;
    void getSingleton();


    void updateActiveObjectMap();

    Ogre::PanelOverlayElement* navMarker_;      //!< the panel used to show the arrow and the target marker
    float navMarkerSize_;                       //!< One paramter size of the navigation marker
    /*
            Ogre::PanelOverlayElement* aimMarker_;      //!< Panel used to show the aim Marker
            float aimMarkerSize_;                       //!< One paramter size of the aim marker
    */
    Ogre::TextAreaOverlayElement* navText_;     //!< Text overlay to display the target distance
    bool wasOutOfView_;                         //!< Performance booster variable: setMaterial is not cheap

    typedef std::map<RadarViewable*, std::pair<Ogre::PanelOverlayElement*, Ogre::TextAreaOverlayElement*> > activeObjectListType;
    activeObjectListType activeObjectList_;
    activeObjectListType::iterator tempRadarViewable;
        
    typedef std::set<RadarViewable*> respawnObjectSetType;
    


    std::string fontName_;
    float textSize_;

};

}

#endif /* _HUDNavigation_H__ */
