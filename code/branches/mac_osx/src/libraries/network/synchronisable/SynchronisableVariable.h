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


#ifndef _SynchronisableVariable_H__
#define _SynchronisableVariable_H__

#include "network/NetworkPrereqs.h"

#include <cassert>
#include <cstring>
#include <loki/TypeTraits.h>

#include "Serialise.h"
#include "core/GameMode.h"
#include "network/synchronisable/NetworkCallbackManager.h"

namespace orxonox{

  namespace VariableDirection{
    enum Value{
      ToClient=0x1,
      ToServer=0x2
    };
  }
  namespace Bidirectionality{
    enum Value{
      ServerMaster=0x1,
      ClientMaster=0x2
    };
  }

  class _NetworkExport SynchronisableVariableBase
  {
    public:
      virtual uint32_t getData(uint8_t*& mem, uint8_t mode)=0;
      virtual void putData(uint8_t*& mem, uint8_t mode, bool forceCallback = false)=0;
      virtual uint32_t getSize(uint8_t mode)=0;
      virtual void* getReference()=0;
      virtual uint8_t getMode()=0;
      virtual ~SynchronisableVariableBase() {}
    protected:
      static uint8_t state_;
  };

  template <class T>
  class SynchronisableVariable: public SynchronisableVariableBase
  {
    public:
      SynchronisableVariable(T& variable, uint8_t syncDirection=VariableDirection::ToClient, NetworkCallbackBase *cb=0);
      virtual ~SynchronisableVariable();

      virtual inline uint8_t getMode(){ return mode_; }
      virtual inline uint32_t getData(uint8_t*& mem, uint8_t mode);
      virtual inline void putData(uint8_t*& mem, uint8_t mode, bool forceCallback = false);
      virtual inline uint32_t getSize(uint8_t mode);
      virtual inline void* getReference(){ return static_cast<void*>(const_cast<typename Loki::TypeTraits<T>::UnqualifiedType*>(&this->variable_)); }
    protected:
      T&                       variable_;
      uint8_t                  mode_;
      NetworkCallbackBase      *callback_;
  };

  template <class T>
  class SynchronisableVariableBidirectional: public SynchronisableVariable<T>
  {
    public:
      SynchronisableVariableBidirectional(T& variable, uint8_t master=Bidirectionality::ServerMaster, NetworkCallbackBase *cb=0);
      virtual ~SynchronisableVariableBidirectional();

      virtual inline uint8_t getMode(){ return 0x3; } //this basically is a hack ^^
      virtual inline uint32_t getData(uint8_t*& mem, uint8_t mode);
      virtual void putData(uint8_t*& mem, uint8_t mode, bool forceCallback = false);
      virtual inline uint32_t getSize(uint8_t mode);
    private:
      T varBuffer_;
      uint8_t varReference_;
  };

  // ================= Unidirectional Part ===============

  template <class T> SynchronisableVariable<T>::SynchronisableVariable(T& variable, uint8_t syncDirection, NetworkCallbackBase *cb):
      variable_( variable ), mode_( syncDirection ), callback_( cb )
  {
    if ( state_ == 0x0 )
    {
      state_ = GameMode::isMaster() ? 0x1 : 0x2;  // set the appropriate mode here
    }
  }

  template <class T> SynchronisableVariable<T>::~SynchronisableVariable()
  {
    if (this->callback_)
    {
      NetworkCallbackManager::deleteCallback(this->callback_); //safe call for deletion
      // this is neccessary because for example for a Vector3 all 3 components of the vector use the same callback
    }
  }

  template <class T> inline uint32_t SynchronisableVariable<T>::getData(uint8_t*& mem, uint8_t mode)
  {
    if ( mode == this->mode_ )
    {
      saveAndIncrease( this->variable_, mem );
      return returnSize( this->variable_ );
    }
    else
      return 0;
  }

  template <class T> void SynchronisableVariable<T>::putData(uint8_t*& mem, uint8_t mode, bool forceCallback)
  {
    assert ( mode == 0x1 || mode == 0x2 );
    bool callback = false;
    if ( mode == this->mode_ ) //don't do anything
      return;
  // check whether we need to consider a callback
    if ( this->callback_ )
    {
      callback = forceCallback || !checkEquality( this->variable_, mem );
    }
  // now do a callback if neccessary
    if ( callback )
    {
      NetworkCallbackManager::triggerCallback( this->callback_ );
    }
  // write the data
    loadAndIncrease( this->variable_, mem );
  }

  template <class T> inline uint32_t SynchronisableVariable<T>::getSize(uint8_t mode)
  {
    if ( mode == this->mode_ )
      return returnSize( this->variable_ );
    else
      return 0;
  }




// ================= Bidirectional Part ================

    template <class T> SynchronisableVariableBidirectional<T>::SynchronisableVariableBidirectional(T& variable, uint8_t master, NetworkCallbackBase *cb):
    SynchronisableVariable<T>( variable, master, cb ), varBuffer_( variable ), varReference_( 0 )
    {
    }

    template <class T> SynchronisableVariableBidirectional<T>::~SynchronisableVariableBidirectional()
    {
    }

    template <class T> uint32_t SynchronisableVariableBidirectional<T>::getData(uint8_t*& mem, uint8_t mode)
    {
      if ( this->mode_ == mode )
      {   // we are master for this variable and have to check whether to change the varReference
        if( this->varBuffer_ != this->variable_ )
        {
          this->varReference_++;
          memcpy(static_cast<void*>(const_cast<typename Loki::TypeTraits<T>::UnqualifiedType*>(&this->varBuffer_)), &this->variable_, sizeof(this->variable_));
        }
      }
  // write the reference number to the stream
      *static_cast<uint8_t*>(mem) = varReference_;
      mem += sizeof(this->varReference_);
  // now write the content
      saveAndIncrease( this->variable_, mem );
      return SynchronisableVariableBidirectional::getSize(mode);
    }

    template <class T> void SynchronisableVariableBidirectional<T>::putData(uint8_t*& mem, uint8_t mode, bool forceCallback)
    {
      bool callback = false;
      if ( this->mode_ == mode )
      {   //        MASTER
        // check that the client (source of the data) has a recent version of this variable
        if ( *static_cast<uint8_t*>(mem) != this->varReference_ )
        { // wrong reference number, so discard the data
//           COUT(0) << "discharding data" << endl;
          mem += getSize( mode ); // SynchronisableVariableBidirectional::getSize returns size of variable + reference
          return;
        }
        else{
          // apply data
          if ( checkEquality( this->variable_, mem+sizeof(varReference_) )==true )
          {
            mem += getSize( mode );
            return;
          }
          else
          {
            mem += sizeof(varReference_);
            memcpy(static_cast<void*>(const_cast<typename Loki::TypeTraits<T>::UnqualifiedType*>(&this->varBuffer_)), &this->variable_, sizeof(T));
            if ( this->callback_ )
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
          if ( checkEquality( this->variable_, mem ) == false )
          {
            // value changed so remark for callback
            if ( this->callback_ )
              callback = true;
          }
        }
      }
  // now do a callback if neccessary
      if ( callback )
      {
        NetworkCallbackManager::triggerCallback( this->callback_ );
      }
  // now write the data
      loadAndIncrease(this->variable_, mem);
    }

    template <class T> inline uint32_t SynchronisableVariableBidirectional<T>::getSize(uint8_t mode)
    {
      return returnSize( this->variable_ ) + sizeof(varReference_);
    }


}


#endif /* _SynchronisableVariable_H__ */
