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
*      Yuning Chai
*   Co-authors:
*      Felix Schulthess
*
*/


#ifndef _BAR_H__
#define _BAR_H__

#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>

#include <OgrePrerequisites.h>
#include "../OrxonoxPrereqs.h"



namespace orxonox
{
  class _OrxonoxExport BarOverlayElement : public Ogre::PanelOverlayElement
  {
    private:

    bool autoColor_;                    // whether bar changes color automatically
    float value_;                       // progress of bar
    int dir_;                           // direction of progress
    int color_;
    int left_;
    int top_;
    int width_;
    int height_;
    int windowW_, windowH_;
    Ogre::Real leftRel_;
    Ogre::Real topRel_;
    Ogre::Real widthRel_;
    Ogre::Real heightRel_;
    Ogre::PanelOverlayElement* bar_;    // the actual bar
    Ogre::OverlayManager* om;           // our overlay manager
    Ogre::OverlayContainer* container_; // our parent container to attach to
    Ogre::String name_;

    public:
        // directions
        static const int RIGHT = 0;
        static const int UP = 1;
        static const int LEFT = 2;
        static const int DOWN = 3;
        // predefined colors
        static const int RED = 0;
        static const int YELLOW = 1;
        static const int GREEN = 2;

        BarOverlayElement(const Ogre::String& name);
        virtual ~BarOverlayElement();

        void init(Real leftRel, Real topRel, Real widthRel, Real heightRel, Ogre::OverlayContainer* container);
        void setDir(int dir);
        void setValue(float value);
        void setColor(int color);

        float getValue();
        int getBarColor();
    };
}
#endif


