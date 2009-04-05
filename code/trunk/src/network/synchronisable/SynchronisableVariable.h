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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */


#ifndef _NETWORK_SYNCHRONISABLEVARIABLE__
#define _NETWORK_SYNCHRONISABLEVARIABLE__

#include "network/NetworkPrereqs.h"

#include <string>
#include <cassert>
#include "util/Math.h"
#include "network/synchronisable/NetworkCallback.h"
#include "network/synchronisable/NetworkCallbackManager.h"

namespace orxonox{
  
  namespace variableDirection{
    enum syncdirection{
      toclient=0x1,
      toserver=0x2
    };
    enum bidirectional{
      serverMaster=0x1,
      clientMaster=0x2
    };
  }
  
  class _NetworkExport SynchronisableVariableBase
  {
    public:
      virtual void getData(uint8_t*& mem, uint8_t mode)=0;
      virtual void putData(uint8_t*& mem, uint8_t mode, bool forceCallback = false)=0;
      virtual uint32_t getSize(uint8_t mode)=0;
      virtual void* getReference()=0;
      virtual uint8_t getMode()=0;
      virtual ~SynchronisableVariableBase() {}
    protected:
      static void setState();
      static uint8_t state_;
  };

  template <class T>
  class SynchronisableVariable: public SynchronisableVariableBase
  {
    public:
      SynchronisableVariable(T& variable, uint8_t syncDirection=variableDirection::toclient, NetworkCallbackBase *cb=0);
      virtual ~SynchronisableVariable();

      virtual inline uint8_t getMode(){ return mode_; }
      virtual inline void getData(uint8_t*& mem, uint8_t mode);
      virtual inline void putData(uint8_t*& mem, uint8_t mode, bool forceCallback = false);
      virtual inline uint32_t getSize(uint8_t mode);
      virtual inline void* getReference(){ return (void *)&this->variable_; }
    protected:
      bool checkEquality(uint8_t* mem);
      void setAndIncrease(uint8_t*& mem);
      void getAndIncrease(uint8_t*& mem);
      uint32_t returnSize();
      
      T& variable_;
      uint8_t mode_;
      NetworkCallbackBase *callback_;
  };
  
  template <class T>
  class SynchronisableVariableBidirectional: public SynchronisableVariable<T>
  {
    public:
      SynchronisableVariableBidirectional(T& variable, uint8_t master=variableDirection::serverMaster, NetworkCallbackBase *cb=0);
      virtual ~SynchronisableVariableBidirectional();
      
      virtual inline uint8_t getMode(){ return 0x3; } //this basically is a hack ^^
      virtual void getData(uint8_t*& mem, uint8_t mode);
      virtual void putData(uint8_t*& mem, uint8_t mode, bool forceCallback = false);
      virtual uint32_t getSize(uint8_t mode);
    private:
      T varBuffer_;
      uint8_t varReference_;
  };

  // ================= Unidirectional Part ===============

  template <class T> SynchronisableVariable<T>::SynchronisableVariable(T& variable, uint8_t syncDirection, NetworkCallbackBase *cb):
      variable_( variable ), mode_( syncDirection ), callback_( cb )
  {
      setState();
  }
  
  template <class T> SynchronisableVariable<T>::~SynchronisableVariable()
  {
    if (this->callback_ != 0)
      NetworkCallbackManager::deleteCallback(this->callback_); //safe call for deletion
  }

  template <class T> void SynchronisableVariable<T>::getData(uint8_t*& mem, uint8_t mode)
  {
    if ( state_ == this->mode_ )
      getAndIncrease( mem );
//   mem += SynchronisableVariable<T>::getSize();
  }

  template <class T> void SynchronisableVariable<T>::putData(uint8_t*& mem, uint8_t mode, bool forceCallback)
  {
    assert ( mode == 0x1 || mode == 0x2 );
    bool callback = false;
    if ( mode == this->mode_ ) //don't do anything
      return;
  // check whether we need to consider a callback
    if ( this->callback_ != 0 )
    {
      if( forceCallback || !checkEquality( mem ) )
        callback = true;
    }
  // write the data
    setAndIncrease( mem );
//   mem += SynchronisableVariable<T>::getSize();
  // now do a callback if neccessary
    if ( callback )
      NetworkCallbackManager::triggerCallback( this->callback_ );
      //this->callback_->call();
  }

  template <class T> uint32_t SynchronisableVariable<T>::getSize(uint8_t mode)
  {
    if ( mode == this->mode_ )
      return returnSize();
    else
      return 0;
  }

  template <> _NetworkExport uint32_t SynchronisableVariable<const bool>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const bool>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const bool>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const bool>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const unsigned char>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const unsigned char>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const unsigned char>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const unsigned char>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const short>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const short>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const short>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const short>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const unsigned short>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const unsigned short>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const unsigned short>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const unsigned short>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const int>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const int>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const int>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const int>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const unsigned int>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const unsigned int>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const unsigned int>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const unsigned int>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const long>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const long>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const long>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const long>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const unsigned long>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const unsigned long>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const unsigned long>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const unsigned long>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const long long>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const long long>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const long long>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const long long>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const unsigned long long>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const unsigned long long>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const unsigned long long>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const unsigned long long>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const float>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const float>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const float>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const float>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const double>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const double>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const double>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const double>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const long double>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const long double>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const long double>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const long double>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const std::string>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const std::string>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const std::string>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const std::string>::checkEquality(uint8_t* mem);
  template <> _NetworkExport uint32_t SynchronisableVariable<const Degree>::returnSize();
  template <> _NetworkExport void     SynchronisableVariable<const Degree>::setAndIncrease(uint8_t*& mem);
  template <> _NetworkExport void     SynchronisableVariable<const Degree>::getAndIncrease(uint8_t*& mem);
  template <> _NetworkExport bool     SynchronisableVariable<const Degree>::checkEquality(uint8_t* mem);



// ================= Bidirectional Part ================

    template <class T> SynchronisableVariableBidirectional<T>::SynchronisableVariableBidirectional(T& variable, uint8_t master, NetworkCallbackBase *cb):
    SynchronisableVariable<T>( variable, master, cb ), varBuffer_( variable ), varReference_( 0 )
    {
    }

    template <class T> SynchronisableVariableBidirectional<T>::~SynchronisableVariableBidirectional()
    {
    }

    template <class T> void SynchronisableVariableBidirectional<T>::getData(uint8_t*& mem, uint8_t mode)
    {
      if ( this->mode_ == mode )
      {   // we are master for this variable and have to check whether to change the varReference
        if( this->varBuffer_ != this->variable_ )
        {
          this->varReference_++;
          memcpy((void*)&this->varBuffer_, &this->variable_, sizeof(this->variable_));
        }
      }
  // write the reference number to the stream
      *static_cast<uint8_t*>(mem) = varReference_;
      mem += sizeof(this->varReference_);
  // now write the content
      SynchronisableVariable<T>::getAndIncrease( mem );
//   mem += SynchronisableVariable<T>::getSize();
    }

    template <class T> void SynchronisableVariableBidirectional<T>::putData(uint8_t*& mem, uint8_t mode, bool forceCallback)
    {
      bool callback = false;
      if ( this->mode_ == mode )
      {   //        MASTER
        // check that the client (source of the data) has a recent version of this variable
        if ( *static_cast<uint8_t*>(mem) != this->varReference_ )
        { // wrong reference number, so discard the data
          mem += getSize( mode ); // SynchronisableVariableBidirectional::getSize returns size of variable + reference
          return;
        }
        else{
          // apply data
          mem += sizeof(varReference_);
          if ( SynchronisableVariableBidirectional<T>::checkEquality( mem )==true )
          {
            mem += SynchronisableVariable<T>::getSize( mode );
            return;
          }
          else
          {
            memcpy((void*)&this->varBuffer_, &this->variable_, sizeof(T));
            if ( this->callback_ != 0 )
              callback = true;
          }
        }
      }
      else
      {   // we are slave for this variable
        if (*static_cast<uint8_t*>(mem) == this->varReference_ && !forceCallback)
        {
          mem += getSize( mode ); //just skip the variable because nothing changed
          return;
        }
        else
        {
          this->varReference_ = *static_cast<uint8_t*>(mem);
          mem += sizeof(varReference_);
          if ( SynchronisableVariable<T>::checkEquality( mem ) == false )
          {
            // value changed so remark for callback
            if ( this->callback_ != 0 )
              callback = true;
          }
        }
      }
  // now write the data
      SynchronisableVariable<T>::setAndIncrease(mem);
  // now do a callback if neccessary
      if ( callback )
        NetworkCallbackManager::triggerCallback( this->callback_ );
        //this->callback_->call();
    }

    template <class T> uint32_t SynchronisableVariableBidirectional<T>::getSize(uint8_t mode)
    {
      return SynchronisableVariable<T>::returnSize() + sizeof(varReference_);
    }
  

}


#endif
