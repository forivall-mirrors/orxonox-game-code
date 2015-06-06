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

#ifndef _CoreStaticInitializationHandler_H__
#define _CoreStaticInitializationHandler_H__

#include "core/CorePrereqs.h"

#include "module/StaticInitializationHandler.h"

namespace orxonox
{
    class _CoreExport CoreStaticInitializationHandler : public StaticInitializationHandler
    {
        friend class Core;

        public:
            CoreStaticInitializationHandler() : bInitInstances_(false) {}

            virtual void setupHandler();
            virtual void shutdownHandler();

            virtual void loadModule(ModuleInstance* module);
            virtual void unloadModule(ModuleInstance* module);

            inline void setInitInstances(bool bInitInstances)
                { this->bInitInstances_ = bInitInstances; }

        private:
            void loadInstances(ModuleInstance* module);
            void initInstances(ModuleInstance* module);

            bool bInitInstances_;
    };
}

#endif /* _CoreStaticInitializationHandler_H__ */
