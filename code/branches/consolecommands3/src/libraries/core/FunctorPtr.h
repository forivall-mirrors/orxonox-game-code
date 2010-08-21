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

#ifndef _FunctorPtr_H__
#define _FunctorPtr_H__

#include "CorePrereqs.h"
#include "SharedPtr.h"

namespace orxonox
{
    typedef SharedPtr<Functor> FunctorPtr;

    typedef SharedChildPtr<FunctorStatic, FunctorPtr> FunctorStaticPtr;

    template <class T>
    class FunctorMemberPtr : public SharedChildPtr<FunctorMember<T>, FunctorPtr>
    {
        public:
            inline FunctorMemberPtr() : SharedChildPtr<FunctorMember<T>, FunctorPtr>() {}
            inline FunctorMemberPtr(FunctorMember<T>* pointer) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(pointer) {}
//            inline FunctorMemberPtr(const FunctorMemberPtr& other) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(other) {}
//            template <class O>
//            inline FunctorMemberPtr(const SharedPtr<O>& other) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(other) {}
//            template <class O>
//            inline FunctorMemberPtr(const SharedChildPtr<O, FunctorMember<T> >& other) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(other) {}
            inline FunctorMemberPtr(const SharedPtr<FunctorMember<T> >& other) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(other) {}

/*
            inline const FunctorMemberPtr& operator=(const FunctorMemberPtr& other) { this->SharedChildPtr<FunctorMember<T>, FunctorPtr>::operator=(other); return *this; }
            template <class O>
            inline const FunctorMemberPtr& operator=(const SharedPtr<O>& other) { this->SharedChildPtr<FunctorMember<T>, FunctorPtr>::operator=(other); return *this; }
*/
        private:
//            inline FunctorMemberPtr(FunctorMember<T>* pointer, int* counter) : SharedChildPtr<FunctorMember<T>, FunctorPtr>(pointer, counter) {}
    };
/*
    typedef SharedChildPtr<FunctorStatic, Functor> FunctorStaticPtr;

    template <class T>
    class FunctorMemberPtr : public SharedChildPtr<FunctorMember<T>, Functor>
    {
        public:
            inline FunctorMemberPtr() : SharedChildPtr<FunctorMember<T>, Functor>() {}
            inline FunctorMemberPtr(FunctorMember<T>* pointer) : SharedChildPtr<FunctorMember<T>, Functor>(pointer) {}
    };
*/
}

#endif /* _FunctorPtr_H__ */
