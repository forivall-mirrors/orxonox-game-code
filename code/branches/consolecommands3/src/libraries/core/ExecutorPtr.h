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

#include "CorePrereqs.h"
#include "SharedPtr.h"

namespace orxonox
{
    typedef SharedPtr<Executor> ExecutorPtr;

    typedef SharedPtr<ExecutorStatic> ExecutorStaticPtr;

    template <class T>
    class ExecutorMemberPtr : public SharedPtr<ExecutorMember<T> >
    {
        public:
            inline ExecutorMemberPtr() : SharedPtr<ExecutorMember<T> >() {}
            inline ExecutorMemberPtr(ExecutorMember<T>* pointer) : SharedPtr<ExecutorMember<T> >(pointer) {}
//            inline ExecutorMemberPtr(const ExecutorMemberPtr& other) : SharedPtr<ExecutorMember<T> >(other) {}
            template <class O>
            inline ExecutorMemberPtr(const SharedPtr<O>& other) : SharedPtr<ExecutorMember<T> >(other) {}
/*
            inline const ExecutorMemberPtr& operator=(const ExecutorMemberPtr& other) { this->SharedPtr<ExecutorMember<T> >::operator=(other); return *this; }
            template <class O>
            inline const ExecutorMemberPtr& operator=(const SharedPtr<O>& other) { this->SharedPtr<ExecutorMember<T> >::operator=(other); return *this; }
*/
        private:
//            inline ExecutorMemberPtr(ExecutorMember<T>* pointer, int* counter) : SharedPtr<ExecutorMember<T> >(pointer, counter) {}
    };
/*
    typedef SharedChildPtr<ExecutorStatic, Executor> ExecutorStaticPtr;

    template <class T>
    class ExecutorMemberPtr : public SharedChildPtr<ExecutorMember<T>, Executor>
    {
        public:
            inline ExecutorMemberPtr() : SharedChildPtr<ExecutorMember<T>, Executor>() {}
            inline ExecutorMemberPtr(ExecutorMember<T>* pointer) : SharedChildPtr<ExecutorMember<T>, Executor>(pointer) {}
    };
*/
}

#endif /* _ExecutorPtr_H__ */
