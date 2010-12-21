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
    @brief Typedefs and definitions of FunctorPtr, FunctorMemberPtr, FunctorStaticPtr, and FunctorPointerPtr

    Instances of orxonox::Functor are usually managed by an orxonox::SharedPtr. This ensures
    that Functors will be destroyed after usage. To make things easier, there's a typedef
    that defines FunctorPtr as SharedPtr<Functor>.

    Because there's not only orxonox::Functor, but also orxonox::FunctorStatic, and
    orxonox::FunctorMember, the shared pointers need to store them all and also reflect
    their hierarchy - FunctorStatic and FunctorMember should not be mixed, but both can
    be converted to Functor. This is achieved by using orxonox::SharedChildPtr.

    Because it's not possible to use a typedef for a template, we have to define the
    helper class FunctorMemberPtr<T> that makes it easier to use FunctorMember. The
    same is also done for FunctorPointerPtr<T>, which can be converted to both,
    FunctorMemberPtr<T> as well as FunctorPtr.
*/

#ifndef _FunctorPtr_H__
#define _FunctorPtr_H__

#include "core/CorePrereqs.h"
#include "util/SharedPtr.h"

namespace orxonox
{
    /// FunctorPtr is just a typedef of SharedPtr
    typedef SharedPtr<Functor> FunctorPtr;

    /// It's not possible to use a typedef for FunctorMemberPtr<T>, so we have to create a child-class instead. It inherits all functions from SharedChildPtr, but needs to (re-)implement some constructors.
    template <class T>
    class FunctorMemberPtr : public SharedChildPtr<FunctorMember<T>, FunctorPtr>
    {
        public:
            inline FunctorMemberPtr() : SharedChildPtr<FunctorMember<T>, FunctorPtr>() {}
            inline FunctorMemberPtr(FunctorMember<T>* pointer) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(pointer) {}
            inline FunctorMemberPtr(const SharedPtr<FunctorMember<T> >& other) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(other) {}
    };

    /// FunctorStaticPtr is just FunctorMemberPtr with @a T = void
    typedef FunctorMemberPtr<void> FunctorStaticPtr;

    /// It's not possible to use a typedef for FunctorPointerPtr<T>, so we have to create a child-class instead. It inherits all functions from SharedChildPtr, but needs to (re-)implement some constructors.
    template <class F, class T>
    class FunctorPointerPtr : public SharedChildPtr<FunctorPointer<F, T>, FunctorMemberPtr<T> >
    {
        public:
            inline FunctorPointerPtr() : SharedChildPtr<FunctorPointer<F, T>, FunctorMemberPtr<T> >() {}
            inline FunctorPointerPtr(FunctorPointer<F, T>* pointer) : SharedChildPtr<FunctorPointer<F, T>, FunctorMemberPtr<T> >(pointer) {}
            inline FunctorPointerPtr(const SharedPtr<FunctorPointer<F, T> >& other) : SharedChildPtr<FunctorPointer<F, T>, FunctorMemberPtr<T> >(other) {}
    };
}

#endif /* _FunctorPtr_H__ */
