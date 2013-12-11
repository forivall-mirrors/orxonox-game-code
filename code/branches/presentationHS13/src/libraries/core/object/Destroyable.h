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
    @ingroup Object
    @brief Declaration of Destroyable, the base class of all objects which can be used with SmartPtr and WeakPtr.
*/

#ifndef _Destroyable_H__
#define _Destroyable_H__

#include "core/CorePrereqs.h"

#include <set>

namespace orxonox
{
    /**
        @brief Classes must inherit from this class if they should be used with SmartPtr or WeakPtr.
    */
    class _CoreExport Destroyable
    {
        template <class T>
        friend class SmartPtr;

        friend class DestructionListener;

        public:
            Destroyable();
            virtual ~Destroyable();

            void destroy();

            /// Returns the number of @ref orxonox::SmartPtr "smart pointers" that point to this object.
            inline unsigned int getReferenceCount() const
                { return this->referenceCount_; }

        protected:
            /// This virtual function is called if destroy() is called and no SmartPtr points to this object. Used in some cases to create a new SmartPtr to prevent destruction.
            virtual void preDestroy() {}

        private:
            /// Increments the reference counter (for smart pointers).
            inline void incrementReferenceCount()
                { ++this->referenceCount_; }
            /// Decrements the reference counter (for smart pointers).
            inline void decrementReferenceCount()
            {
                --this->referenceCount_;
                if (this->referenceCount_ == 0 && this->requestedDestruction_)
                    this->destroy();
            }

            /// Register a destruction listener (for example a weak pointer which points to this object).
            inline void registerDestructionListener(DestructionListener* pointer)
                { this->destructionListeners_.insert(pointer); }
            /// Unegister a destruction listener (for example a weak pointer which pointed to this object before).
            inline void unregisterDestructionListener(DestructionListener* pointer)
                { this->destructionListeners_.erase(pointer); }

            int referenceCount_;                                    //!< Counts the references from smart pointers to this object
            bool requestedDestruction_;                             //!< Becomes true after someone called delete on this object
            std::set<DestructionListener*> destructionListeners_;   //!< All destruction listeners (for example weak pointers which point to this object and like to get notified if it dies)
    };

    /**
        @brief This listener is used to inform weak pointers if an object of type Destroyable gets destroyed.
    */
    class _CoreExport DestructionListener
    {
        friend class Destroyable;

        protected:
            virtual ~DestructionListener() {}

            inline void registerAsDestructionListener(Destroyable* object)
                { if (object) { object->registerDestructionListener(this); } }
            inline void unregisterAsDestructionListener(Destroyable* object)
                { if (object) { object->unregisterDestructionListener(this); } }

            virtual void objectDeleted() = 0;
    };

}

#endif /* _Destroyable_H__ */
