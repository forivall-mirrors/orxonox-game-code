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

#ifndef _ExecutorPtr_H__
#define _ExecutorPtr_H__

#include "core/CorePrereqs.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    typedef SharedPtr<Executor> ExecutorPtr;

    typedef SharedChildPtr<ExecutorStatic, ExecutorPtr> ExecutorStaticPtr;

    template <class T>
    class ExecutorMemberPtr : public SharedChildPtr<ExecutorMember<T>, ExecutorPtr>
    {
        public:
            inline ExecutorMemberPtr() : SharedChildPtr<ExecutorMember<T>, ExecutorPtr>() {}
            inline ExecutorMemberPtr(ExecutorMember<T>* pointer) : SharedChildPtr<ExecutorMember<T>, ExecutorPtr>(pointer) {}
            inline ExecutorMemberPtr(const SharedPtr<ExecutorMember<T> >& other) : SharedChildPtr<ExecutorMember<T>, ExecutorPtr>(other) {}
    };
}

#endif /* _ExecutorPtr_H__ */
