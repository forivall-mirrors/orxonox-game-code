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
    @ingroup Command Tcl
    @brief Definition of TclThreadList.
*/

#ifndef _TclThreadList_H__
#define _TclThreadList_H__

#include "core/CorePrereqs.h"

#include <list>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace orxonox
{
    /**
        @brief A thread-safe implementation of a message queue, used by TclThreadManager.
    */
    template <class T>
    class TclThreadList
    {
        public:
            void push_front(const T& value);
            void push_back(const T& value);
            template <class InputIterator> void insert(typename std::list<T>::iterator position, InputIterator begin, InputIterator end);

            void wait_and_pop_front(T* value);
            void wait_and_pop_back(T* value);
            bool try_pop_front(T* value);
            bool try_pop_back(T* value);
            void clear();

            size_t size() const;
            bool empty() const;
            bool is_in(const T& value) const;

            /**
                @brief Returns a reference to the list. Don't forget to lock the mutex (see @ref getMutex).
            */
            inline std::list<T>& getList()
                { return this->list_; }

            /**
                @brief Returns a reference to the list. Don't forget to lock the mutex (see @ref getMutex).
            */
            inline const std::list<T>& getList() const
                { return this->list_; }

            /**
                @brief Returns a reference to the mutex which might be useful if you want to iterate through the list (see @ref getList()).
            */
            inline boost::shared_mutex& getMutex() const
                { return this->mutex_; }

        private:
            std::list<T>                  list_;        ///< A standard list for type T
            mutable boost::shared_mutex   mutex_;       ///< A mutex to grant exclusive access to the list
            boost::condition_variable_any condition_;   ///< A condition variable to wake threads waiting for the mutex to become ready
    };

    /**
        @brief Pushes a new element to the front of the list. A unique_lock is needed.
    */
    template <class T>
    void TclThreadList<T>::push_front(const T& value)
    {
        boost::unique_lock<boost::shared_mutex> lock(this->mutex_);
        this->list_.push_front(value);
        lock.unlock();                  // unlock the mutex...
        this->condition_.notify_all();  // ...then call notify_all to wake threads waiting in wait_and_pop_front/back
    }

    /**
        @brief Pushes a new element to the back of the list. A unique_lock is needed.
    */
    template <class T>
    void TclThreadList<T>::push_back(const T& value)
    {
        boost::unique_lock<boost::shared_mutex> lock(this->mutex_);
        this->list_.push_back(value);
        lock.unlock();                  // unlock the mutex...
        this->condition_.notify_all();  // ...then call notify_all to wake threads waiting in wait_and_pop_front/back
    }

    /**
        @brief Inserts new elements into the list. A unique_lock is needed.
    */
    template <class T>
    template <class InputIterator> void TclThreadList<T>::insert(typename std::list<T>::iterator position, InputIterator begin, InputIterator end)
    {
        boost::unique_lock<boost::shared_mutex> lock(this->mutex_);
        this->list_.insert(position, begin, end);
        lock.unlock();                  // unlock the mutex...
        this->condition_.notify_all();  // ...then call notify_all to wake threads waiting in wait_and_pop_front/back
    }

    /**
        @brief Waits until the list contains at least one element and then pops and returns the front element.
        @param value The front value will be stored in the variable referenced by this pointer.
    */
    template <class T>
    void TclThreadList<T>::wait_and_pop_front(T* value)
    {
        boost::unique_lock<boost::shared_mutex> lock(this->mutex_);

        while (this->list_.empty())       // check if there's an element in the list
            this->condition_.wait(lock);  // wait until the condition becomes true (a notification comes from push_front, push_back or insert

        *value = this->list_.front();
        this->list_.pop_front();
    }

    /**
        @brief Waits until the list contains at least one element and then pops and returns the back element.
        @param value The back value will be stored in the variable referenced by this pointer.
    */
    template <class T>
    void TclThreadList<T>::wait_and_pop_back(T* value)
    {
        boost::unique_lock<boost::shared_mutex> lock(this->mutex_);

        while (this->list_.empty())       // check if there's an element in the list
            this->condition_.wait(lock);  // wait until the condition becomes true (a notification comes from push_front, push_back or insert

        *value = this->list_.back();
        this->list_.pop_back();
    }

    /**
        @brief Pops and returns the front element if there's at least one element in the list.
        @param value The front value will be stored in the variable referenced by this pointer.
        @return Returns true if there was at least one element in the list (which got poped). If the list was empty, false is returned immediately.

        Needs a unique_lock if there's an element to pop. If not, shared_lock is sufficient.
    */
    template <class T>
    bool TclThreadList<T>::try_pop_front(T* value)
    {
        boost::upgrade_lock<boost::shared_mutex> lock(this->mutex_); // gain shared lock

        if (this->list_.empty())
        {
            // No elements - return immediately
            return false;
        }
        else
        {
            // At least one element - write it into the passed variable and pop it from the list
            boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock); // upgrade to unique lock to modify the list
            *value = this->list_.front();
            this->list_.pop_front();
        }
        return true;
    }

    /**
        @brief Pops and returns the back element if there's at least one element in the list.
        @param value The back value will be stored in the variable referenced by this pointer.
        @return Returns true if there was at least one element in the list (which got poped). If the list was empty, false is returned immediately.

        Needs a unique_lock if there's an element to pop. If not, shared_lock is sufficient.
    */
    template <class T>
    bool TclThreadList<T>::try_pop_back(T* value)
    {
        boost::upgrade_lock<boost::shared_mutex> lock(this->mutex_); // gain shared lock

        if (this->list_.empty())
        {
            // No elements - return immediately
            return false;
        }
        else
        {
            // At least one element - write it into the passed variable and pop it from the list
            boost::upgrade_to_unique_lock<boost::shared_mutex> unique_lock(lock); // upgrade to unique lock to modify the list
            *value = this->list_.back();
            this->list_.pop_back();
        }
        return true;
    }

    /**
        @brief Clears the list. A unique_lock is needed.
    */
    template <class T>
    void TclThreadList<T>::clear()
    {
        boost::unique_lock<boost::shared_mutex> lock(this->mutex_);
        this->list_.clear();
    }

    /**
        @brief Returns the size of the list. A shared_lock is needed.

        Warning: Don't change the list based on the result of size(). Use an atomic function instead. Other threads may change the list
        beween your call to size() and your further actions, so be careful and use this function only if you really want nothing else than
        just the size of the list.
    */
    template <class T>
    size_t TclThreadList<T>::size() const
    {
        boost::shared_lock<boost::shared_mutex> lock(this->mutex_);
        return this->list_.size();
    }

    /**
        @brief Returns true if the list is empty, false otherwise. A shared_lock is needed.

        Warning: Don't change the list based on the result of empty(). Use an atomic function instead. Other threads may change the list
        beween your call to empty() and your further actions, so be careful and use this function only if you really want nothing else than
        just if the list is empty or not.
    */
    template <class T>
    bool TclThreadList<T>::empty() const
    {
        boost::shared_lock<boost::shared_mutex> lock(this->mutex_);
        return this->list_.empty();
    }

    /**
        @brief Returns true if a given element is in the list, false otherwise. A shared_lock is needed.

        Warning: The result of this function might be wrong just one instruction after the call. Use this function just to get information
        about a temporary snapshot and don't change the list based on the result of this function.
    */
    template <class T>
    bool TclThreadList<T>::is_in(const T& value) const
    {
        boost::shared_lock<boost::shared_mutex> lock(this->mutex_);

        for (typename std::list<T>::const_iterator it = this->list_.begin(); it != this->list_.end(); ++it)
            if (*it == value)
                return true;

        return false;
    }
}

#endif /* _TclThreadList_H__ */
