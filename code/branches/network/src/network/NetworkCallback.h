#ifndef _NETWORK_CALLBACK__
#define _NETWORK_CALLBACK__

namespace network{
  class NetworkCallbackBase
  {
    public:
      virtual void call() = 0;
  };
  
  template <class T>
  class NetworkCallback: public NetworkCallbackBase
  {
    public:
      NetworkCallback(T* object, void (T::*function) (void)) : object_(object), function_(function) {}
      virtual void call()
        { (this->object_->*function_)(); }
  
    private:
      T* object_;
      void (T::*function_) (void);
  }; 


}


#endif
