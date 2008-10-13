#ifndef _NETWORK_CALLBACK__
#define _NETWORK_CALLBACK__

#include "NetworkPrereqs.h"

namespace network{
  class _NetworkExport NetworkCallbackBase
  {
    public:
      virtual void call() = 0;
	  virtual ~NetworkCallbackBase() {}
  };

  template <class T>
  class _NetworkExport NetworkCallback: public NetworkCallbackBase
  {
    public:
      NetworkCallback(T* object, void (T::*function) (void)) : object_(object), function_(function) {}
	  virtual ~NetworkCallback() {}
      virtual void call()
        { (this->object_->*function_)(); }

    private:
      T* object_;
      void (T::*function_) (void);
  };


}


#endif
