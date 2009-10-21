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
#ifndef _AmbientSound_H__
#define _AmbientSound_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"
#include "sound/BaseSound.h"

namespace orxonox
{
    /**
     * The AmbientSound class is the base class for all sound file loader classes.
     * It server as main interface to the OpenAL library.
     *
     */
    class _OrxonoxExport AmbientSound : public BaseSound, public BaseObject
    {
    public:
        AmbientSound(BaseObject* creator);
        virtual ~AmbientSound();

        virtual void play();
        virtual void replay();      // is only needed for the AmbientSound list in SoundManager
        virtual void stop();
        virtual void pause();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
        virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
        virtual void changedActivity();


    private:
    };
}

#endif /* _AmbientSound_H__ */
