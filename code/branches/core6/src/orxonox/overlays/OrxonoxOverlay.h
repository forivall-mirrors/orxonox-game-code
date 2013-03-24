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

/**
@file
@brief Declaration of the OrxonoxOverlay class.
*/

#ifndef _OrxonoxOverlay_H__
#define _OrxonoxOverlay_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include "util/Math.h"
#include "util/OgreForwardRefs.h"
#include "core/BaseObject.h"
#include "core/WindowEventListener.h"
#include "core/class/Super.h"

namespace orxonox
{
    /**
    @brief
        Base class to display content directly onto the screen.
        This is merely a wrapper of the Ogre::Overlay to implement more features and integrate it
        in our class hierarchy for xml loading and config values.
        The mentioned features are:
        - Automatic positioning depending on the scale and the rotation angle.
          You can specify a "pick point" relative to the overlay itself. This point will always be exactly
          at the position (position_) of the overlay. That allows for margin/corner aligment.
          It even works when a rotation angle is applied.
        - Virtual methods for changedVisibilty() (BaseObject), angleChanged(), sizeCorrectionChanged(),
          sizeChanged() and positionChanged(), that can be overridden by any derivative. This enables for
          custom configurability of the size, position and rotation attributes. For intance, the HUDNavigation
          should behave differently to sizeChanged() than a standard overlay.
        - Console commands for scale, rotate and scroll (accessed by name)
        - Standard Ogre::PanelOverlayElement for a background image (class doesn't have to be derived
          only for displaying a picture).
        - Reacts to changes of the window aspect
        - Last but not least: On demand you can tell the overlay to automatically resale to correct for
          aspect distortion. E.g. if you play 1024x768 you wouldn't want a round object to be oval.
          Remark: This can (due to the Ogre::Overlay transformation order) only work for angle that are
                  multiples of 90 degrees. But it's only a small drawback.
    */
    class _OrxonoxExport OrxonoxOverlay : public BaseObject, public WindowEventListener
    {
    public:
        /**
        @brief
            Describes the rotational state of a an overlay.
            Horizontal means 0/180 degrees, Vertical is 90/270 degrees
            and in between is everything else.
        */
        enum RotationState
        {
            Horizontal,
            Vertical,
            Inbetween
        };

    public:
        OrxonoxOverlay(BaseObject* creator);
        virtual ~OrxonoxOverlay();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        virtual void changedName();

        //! Shows the overlay with an detour to BaseObject::visibility_
        void show() { this->setVisible(true); }
        //! Hides the overlay with an detour to BaseObject::visibility_
        void hide() { this->setVisible(false); }

        /** Sets whether the aspect of the overlay is corrected.
            This is for instance useful for round objects that should stay round no matter
            what the screen resolution is. */
        void setAspectCorrection(bool val)        { this->bCorrectAspect_ = val; this->sizeCorrectionChanged(); }
        //! Returns whether the window aspect is corrected
        bool getAspectCorrection() const          { return this->bCorrectAspect_; }

        //! Sets the position of this overlay on the screen.
        void setPosition(Vector2 pos)             { this->position_ = pos; this->positionChanged(); }

        //! Returns the current position on the screen.
        const Vector2& getPosition() const        { return this->position_; }

        //! Scrolls the overlay. @param offset The offset given.
        void scroll(const Vector2& offset)        { this->position_ += offset; this->positionChanged(); }

        /** Sets the point in the overlay where to pick it when translating.
            For instance setting it to (1.0,1.0) means that the lower right corner of the
            overlay will be put at position_.
            This primarily helps aligning an overlay to any corner/margin on the screen. */
        void setPickPoint(const Vector2& position){ this->pickPoint_ = position; this->positionChanged(); }

        //! Gets the pick point of this overlay. @see setPickPoint()
        const Vector2& getPickPoint() const       { return this->pickPoint_; }

        //! Sets the rotation angle applied to this overlay in degrees.
        void setRotation(const Degree& angle)     { this->angle_ = angle; this->angleChanged(); }

        //! Gets the rotation angle applied to this overlay in degrees.
        const Degree& getRotation() const         { return this->angle_; }

        //! Rotates the overlay by angle degrees.
        void rotate(const Degree& angle)          { this->angle_ += angle; this->angleChanged(); }

        //! Sets the size of this overlay.
        void setSize(const Vector2& size)         { this->size_ = size; this->sizeChanged(); }

        //! Gets the current size that was set (uncorrected)
        const Vector2& getSize() const            { return this->size_; }

        //! Gets the actual size of the overlay on the screen (corrected)
        Vector2 getActualSize() const             { return this->size_ * this->sizeCorrection_; }

        //! Gets the current size correction (default: 1.0, 1.0)
        const Vector2& getSizeCorrection() const  { return this->sizeCorrection_; }

        //! Scales the overlay by scale.
        void scale(const Vector2& scale)          { this->size_ *= scale; this->sizeChanged(); }

        //! ConsoleCommand: Accesses the overlay by its name and scales it.
        static void scaleOverlay(const std::string& name, float scale);
        //! ConsoleCommand: Accesses the overlay by its name and scrolls it.
        static void scrollOverlay(const std::string& name, const Vector2& scroll);
        static void toggleVisibility(const std::string& name);
        static void showOverlay(const std::string& name);
        //! ConsoleCommand: Accesses the overlay by its name and rotates it.
        static void rotateOverlay(const std::string& name, const Degree& angle);

        void setBackgroundMaterial(const std::string& material);
        const std::string& getBackgroundMaterial() const;

        void setBackgroundTexture(const std::string& texture);
        const std::string& getBackgroundTexture() const;

        void setBackgroundAlpha(float alpha);

        void setBackgroundColour(ColourValue colour);

        virtual void changedVisibility();

        inline void setOwner(BaseObject* owner)
        {
            if (this->owner_ != owner)
            {
                this->owner_ = owner;
                this->changedOwner();
            }
        }
        inline BaseObject* getOwner() const
            { return this->owner_; }
        virtual void changedOwner() {}

        void setOverlayGroup(OverlayGroup* group);
        inline OverlayGroup* getOverlayGroup() const
            { return this->group_; }
        virtual void changedOverlayGroup()
            { this->changedVisibility(); }

    protected:
        virtual void angleChanged();
        virtual void sizeCorrectionChanged();
        virtual void sizeChanged();
        virtual void positionChanged();

        Ogre::Overlay* overlay_;                   //!< The overlay the entire class is about.
        Ogre::PanelOverlayElement* background_;    //!< Background image (blank per default).

        float windowAspectRatio_;                  //!< Screen.width / screen.height
        bool bCorrectAspect_;                      //!< Whether or not to correct the size. @see setAspectCorrection()
        Vector2 size_;                             //!< Internal size of the overlay.
        Vector2 sizeCorrection_;                   //!< Value to correct the size because of the window aspect.
        Vector2 position_;                         //!< Position of the pickPoint on the screen.
        Vector2 pickPoint_;                        //!< Point on the overlay to pick when translating
        Degree angle_;                             //!< Rotation angle of the overlay
        RotationState rotState_;                   //!< horizontal, vertical or inbetween

    private:
        void windowResized(unsigned int newWidth, unsigned int newHeight);

        static unsigned int hudOverlayCounter_s;   //!< Static counter for hud elements
        /** Contains all the overlays in a map for quick access via console commands.
            We could also use the ObjectList, but that doesn't guarantee XMLPort(.) was called and is slower. */
        static std::map<std::string, OrxonoxOverlay*> overlays_s;
        BaseObject* owner_;
        OverlayGroup* group_;
        Ogre::Pass* backgroundAlphaPass_;
  };

  SUPER_FUNCTION(6, OrxonoxOverlay, changedOwner, false);
  SUPER_FUNCTION(7, OrxonoxOverlay, changedOverlayGroup, false);
}

#endif /* _OrxonoxOverlay_H__ */
