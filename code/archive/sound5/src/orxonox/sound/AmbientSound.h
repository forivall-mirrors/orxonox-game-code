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
 *      
 *   Co-authors:
 *      Kevin Young
 *
 */

#ifndef _AmbientSound_H__
#define _AmbientSound_H__

#include <boost/thread.hpp>

#include "sound/SoundPrereqs.h"

#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"
#include "BaseSound.h"
#include "MoodManager.h"

namespace orxonox
{
    /**
     * The AmbientSound class implements the non-3D sound, i.e. sound files that are used for atmospheric
     * highlighting.
     * It interfaces with BaseSound and is controllable by MoodManager.
     * Ambient sounds are always cross-faded. New sounds are registered and activated/deactivated as needed.
     *
     */
    class _OrxonoxExport AmbientSound : public BaseSound, public BaseObject, public Synchronisable, public MoodListener
    {
        friend class SoundManager;

    public:
        AmbientSound(BaseObject* creator);

        void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
        void changedActivity();

        void play();
        void stop();
        void pause();

        void setAmbientSource(const std::string& source);
        inline const std::string& getAmbientSource() const
            { return this->ambientSource_; }

        void setPlayOnLoad(bool val);
        bool getPlayOnLoad() const
            { return this->bPlayOnLoad_; }

    protected:
        ~AmbientSound() { }

        void doStop();
        void doPlay();

    private:
        void preDestroy();
        void registerVariables();
        float getRealVolume();
        void moodChanged(const std::string& mood);
        inline void ambientSourceChanged()
            { this->setAmbientSource(this->ambientSource_); }
        inline void playOnLoadChanged()
            { this->setPlayOnLoad(this->bPlayOnLoad_); }

        std::string ambientSource_; //!< Analogous to source_, but mood independent
        bool        bPlayOnLoad_;   //!< Play the sound immediately when loaded

        boost::thread soundstreamthread_; // hacky solution for streaming
        void setStreamSource(const std::string& source);
    };
}

#endif /* _AmbientSound_H__ */
