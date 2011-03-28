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
 *      Kevin Young
 *   Co-authors:
 *      ...
 *
 */

#ifndef _AmbientSound_H__
#define _AmbientSound_H__

#include "OrxonoxPrereqs.h"

#include "BaseSound.h"
#include "MoodManager.h"

namespace orxonox
{
    /**
     * The AmbientSound class is used to play background music. It can not be placed
     * directly in a level file, use WorldAmbientSound instead.
     */
    class _OrxonoxExport AmbientSound : public BaseSound, public MoodListener
    {
        friend class SoundManager;
        friend class WorldAmbientSound;

    public:
        AmbientSound();

        void play();
        bool stop();
        void pause();

        void setAmbientSource(const std::string& source);
        inline const std::string& getAmbientSource() const
            { return this->ambientSource_; }

        void setPlayOnLoad(bool val);
        bool getPlayOnLoad() const
            { return this->bPlayOnLoad_; }

    protected:
        ~AmbientSound() { }

    private:
        void preDestroy();
        float getRealVolume();
        void moodChanged(const std::string& mood);
        inline void ambientSourceChanged()
            { this->setAmbientSource(this->ambientSource_); }
        inline void playOnLoadChanged()
            { this->setPlayOnLoad(this->bPlayOnLoad_); }

        std::string ambientSource_; //!< Analogous to source_, but mood independent
        bool        bPlayOnLoad_;   //!< Play the sound immediately when loaded
    };
}

#endif /* _AmbientSound_H__ */
