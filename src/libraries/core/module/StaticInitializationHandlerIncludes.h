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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _StaticInitializationHandlerIncludes_H__
#define _StaticInitializationHandlerIncludes_H__

#include "core/CorePrereqs.h"

#include "StaticInitializationHandler.h"
#include "StaticallyInitializedInstance.h"

/**
 * Registers a StaticInitializationHandler
 */
#define RegisterStaticInitializationHandler(className) \
    static StaticInitializationHandler& className##StaticInitializationHandler \
        = (new orxonox::SI_SIH(new className()))->getHandler()

namespace orxonox
{
    class _CoreExport StaticallyInitializedStaticInitializationHandler : public StaticallyInitializedInstance
    {
        public:
            StaticallyInitializedStaticInitializationHandler(StaticInitializationHandler* handler)
                : StaticallyInitializedInstance(0)
                , handler_(handler)
            {}
            ~StaticallyInitializedStaticInitializationHandler() { delete handler_; }

            virtual void load();
            virtual void unload();

            inline StaticInitializationHandler& getHandler()
                { return *this->handler_; }

        private:
            StaticInitializationHandler* handler_;
    };

    typedef StaticallyInitializedStaticInitializationHandler SI_SIH;
}

#endif /* _StaticInitializationHandlerIncludes_H__ */
