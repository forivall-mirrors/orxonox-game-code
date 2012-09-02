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

/**
    @file
    @ingroup Command FunctorExecutor
    @brief Typedefs and definitions of ExecutorPtr, ExecutorStaticPtr, and ExecutorMemberPtr

    Instances of orxonox::Executor are usually managed by an orxonox::SharedPtr. This ensures
    that Executors will be destroyed after usage. To make things easier, there's a typedef
    that defines ExecutorPtr as SharedPtr<Executor>.

    Because there's not only orxonox::Executor, but also orxonox::ExecutorStatic, and
    orxonox::ExecutorMember, the shared pointers need to store them all and also reflect
    their hierarchy - ExecutorStatic and ExecutorMember should not be mixed, but both can
    be converted to Executor. This is achieved by using orxonox::SharedChildPtr.

    Because it's not possible to use a typedef for a template, we have to define the
    helper class ExecutorMemberPtr<T> that makes it easier to use ExecutorMember.
*/

#ifndef _ExecutorPtr_H__
#define _ExecutorPtr_H__

#include "core/CorePrereqs.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    /// ExecutorPtr is just a typedef of SharedPtr
    typedef SharedPtr<Executor> ExecutorPtr;

    /// ExecutorStaticPtr is just a typedef of SharedChildPtr
    typedef SharedChildPtr<ExecutorStatic, ExecutorPtr> ExecutorStaticPtr;

    /// It's not possible to use a typedef for ExecutorMemberPtr<T>, so we have to create a child-class instead. It inherits all functions from SharedChildPtr, but needs to (re-)implement some constructors.
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
