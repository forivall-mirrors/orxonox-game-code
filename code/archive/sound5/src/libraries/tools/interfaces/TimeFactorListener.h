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

#ifndef _TimeFactorListener_H__
#define _TimeFactorListener_H__

#include "tools/ToolsPrereqs.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _ToolsExport TimeFactorListener : virtual public OrxonoxClass
    {
        public:
            TimeFactorListener();
            virtual ~TimeFactorListener() {}

            static void setTimeFactor( float factor );
            static void setTimeFactorInternal( float factor );
            static inline float getTimeFactor(){ return TimeFactorListener::timefactor_s; }

        protected:
            virtual void changedTimeFactor(float factor_new, float factor_old) {}

        private:
            static float timefactor_s;
    };
}

#endif /* _TimeFactorListener_H__ */
