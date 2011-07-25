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

#ifndef _KeyDetector_H__
#define _KeyDetector_H__

#include "InputPrereqs.h"

#include <boost/shared_ptr.hpp>
#include "util/Singleton.h"
#include "KeyBinder.h"

namespace orxonox
{
    class _CoreExport KeyDetector : public KeyBinder, public Singleton<KeyDetector>
    {
        friend class Singleton<KeyDetector>;

    public:
        KeyDetector();
        ~KeyDetector();

        void setCallback(const shared_ptr<Functor>& function) { this->callbackFunction_ = function; }

    private:
        KeyDetector(const KeyDetector&);

        void callback(const std::string& name);
        void JoyStickQuantityChanged(const std::vector<JoyStick*>& joyStickList);
        void assignCommands();

        shared_ptr<Functor> callbackFunction_;
        InputState* inputState_;
        static std::string callbackCommand_s;
        static KeyDetector* singletonPtr_s;
    };
}

#endif /* _KeyDetector_H__ */
