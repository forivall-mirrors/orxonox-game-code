#ifndef _Timer_H__
#define _Timer_H__

#include "../core/CoreIncludes.h"
#include "OgreFrameListener.h"

namespace orxonox
{
    class TimerBase : public OrxonoxClass
    {
        friend class TimerFrameListener;

        public:
            TimerBase()
            {
                RegisterRootObject(TimerBase);

                this->interval_ = 0;
                this->bLoop_ = false;
                this->bActive_ = false;

                this->time_ = 0;
            }

            virtual void run() const = 0;

            inline void startTimer() { this->bActive_ = true; this->time_ = this->interval_; }
            inline void stopTimer() { this->bActive_ = false; this->time_ = this->interval_; }
            inline void pauseTimer() { this->bActive_ = false; }
            inline void unpauseTimer() { this->bActive_ = true; }
            inline bool isActive() const { return this->bActive_; }

        protected:
            float interval_;
            bool bLoop_;
            bool bActive_;

            float time_;
    };

    template <class T = BaseObject>
    class Timer : public TimerBase
    {
        public:
            Timer()
            {
                this->timerFunction_ = 0;
                this->object_ = 0;
            }

            Timer(float interval, bool bLoop, T* object, void (T::*timerFunction)())
            {
                this->setTimer(interval, bLoop, timerFunction, object);
            }

            void setTimer(float interval, bool bLoop, T* object, void (T::*timerFunction)())
            {
                this->interval_ = interval;
                this->bLoop_ = bLoop;
                this->timerFunction_ = timerFunction;
                this->object_ = object;
                this->bActive_ = true;

                this->time_ = interval;
            }

            void run() const
            {
                ((*this->object_).*timerFunction_)();
            }

        private:
            void (T::*timerFunction_)();
            T* object_;
    };

    class TimerFrameListener : public Ogre::FrameListener
    {
        private:
            bool frameStarted(const Ogre::FrameEvent &evt)
            {
                for (Iterator<TimerBase> it = ObjectList<TimerBase>::start(); it; ++it)
                {
                    if (it->isActive())
                    {
                        it->time_ -= evt.timeSinceLastFrame;

                        if (it->time_ <= 0)
                        {
                            if (it->bLoop_)
                                it->time_ += it->interval_;
                            else
                                it->stopTimer();

                            it->run();
                        }
                    }
                }

                return FrameListener::frameStarted(evt);
            }
    };
}

#endif
