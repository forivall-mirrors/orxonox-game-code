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
#include "Executor.h"

namespace orxonox
{
    boost::posix_time::millisec THREAD_WAIT_BEFORE_DETACH(1000);


    Thread::Thread():
        executor_(0),
        isWorking_(false),
        stopThread_(false)
    {
        this->executorMutex_ = new boost::mutex;
        this->isWorkingMutex_ = new boost::mutex;
        this->stopThreadMutex_ = new boost::mutex;
        this->workerThread_ = new boost::thread( boost::bind(&Thread::threadLoop, this) );
    }

    Thread::~Thread()
    {
        this->stopThreadMutex_->lock();
        this->stopThread_ = true;
        this->stopThreadMutex_->unlock();
        if( !this->workerThread_->timed_join( THREAD_WAIT_BEFORE_DETACH ) )
            assert(0); // this should not happen
        delete this->workerThread_;
        delete this->executorMutex_;
        delete this->stopThreadMutex_;
        delete this->isWorkingMutex_;
    }

    bool Thread::isWorking()
    {
      this->isWorkingMutex_->lock();
      bool isWorking = this->isWorking_;
      this->isWorkingMutex_->unlock();
      return isWorking;
    }

    bool Thread::evaluateExecutor( Executor* executor )
    {
        this->isWorkingMutex_->lock();
        this->isWorking_=true;
        this->isWorkingMutex_->unlock();
        this->executorMutex_->lock();
        this->executor_ = executor;
        this->executorMutex_->unlock();
        return true;
    }

    void Thread::threadLoop()
    {
        bool stopThread = false;
        while( !stopThread )
        {
            //this->executorMutex_->lock();
            Executor* executor = this->executor_;
            //this->executorMutex_->unlock();
            if( executor )
            {
                (*executor)();
                this->executorMutex_->lock();
                delete this->executor_;
                this->executor_ = 0;
                this->executorMutex_->unlock();
                this->isWorkingMutex_->lock();
                this->isWorking_=false;
                this->isWorkingMutex_->unlock();
            }
            else
            {
                this->workerThread_->yield();
            }
            //this->stopThreadMutex_->lock();
            stopThread = this->stopThread_;
            //this->stopThreadMutex_->unlock();
        }
    }

    void Thread::waitUntilFinished()
    {
        bool stillWorking = true;
        while( stillWorking )
        {
            this->isWorkingMutex_->lock();
            stillWorking = this->isWorking_;
            this->isWorkingMutex_->unlock();
            if( stillWorking )
                msleep( 1 );
        }
    }
}
