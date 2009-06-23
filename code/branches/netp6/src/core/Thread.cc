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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "Thread.h"

#include <cassert>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread_time.hpp>

#include "util/Sleep.h"
#include "Functor.h"

namespace orxonox
{
    boost::posix_time::millisec THREAD_WAIT_BEFORE_DETACH(1000);
    
    
    Thread::Thread():
        functor_(0),
        isWorking_(false),
        stopThread_(false)
    {
        this->communicationMutex_ = new boost::mutex;
        this->workerThread_ = new boost::thread( boost::bind(&Thread::threadLoop, this) );
    }
    
    Thread::~Thread()
    {
        this->stopThread_ = true;
        if( !this->workerThread_->timed_join( THREAD_WAIT_BEFORE_DETACH ) )
            assert(0); // this should not happen
        delete this->workerThread_;
        delete this->communicationMutex_;
    }
    
    bool Thread::evaluateFunctor( Functor* functor )
    {
        if( this->communicationMutex_->try_lock() )
        {
            this->functor_ = functor;
            this->communicationMutex_->unlock();
            return true;
        }
        else
            return false;
    }
    
    void Thread::threadLoop()
    {
        while( !this->stopThread_ )
        {
            this->communicationMutex_->lock();
            if( this->functor_ )
            {
                (*this->functor_)();
                this->communicationMutex_->unlock();
            }
            else
            {
                this->communicationMutex_->unlock();
                this->workerThread_->yield();
            }
        }
    }
    
    void Thread::waitUntilFinished()
    {
        bool stillWorking = true;
        while( stillWorking )
        {
            this->communicationMutex_->lock();
            stillWorking = this->isWorking_;
            this->communicationMutex_->unlock();
            if( stillWorking )
                msleep( 1 );
        }
    }
}
