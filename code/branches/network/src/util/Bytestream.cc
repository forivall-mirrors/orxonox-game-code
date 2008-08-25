#include "Bytestream.h"
#include <assert.h>

#define DEFAULT_SIZE 100 // bytes
#define RESIZE_AMOUNT 100

  Bytestream::Bytestream(){
    data_ = new DATA_TYPE[DEFAULT_SIZE+1];
    readIterator_ = 0;
    writeIterator_ = 0;
    maxSize_ = DEFAULT_SIZE;
  }
  
  Bytestream::Bytestream( unsigned int size ){
    data_ = new DATA_TYPE[ size+1 ];
    readIterator_ = 0;
    writeIterator_ = 0;
    maxSize_ = size;
  }
  
  Bytestream::Bytestream( DATA_TYPE *data, unsigned int size, bool empty ){
    data_ = data;
    if(empty)
      writeIterator_ = 0;
    else
      writeIterator_ = size;
    readIterator_ = 0;
    maxSize_ = size;
  }
  
  Bytestream::~Bytestream(){
    delete [] data_;
  }
  
  
  void Bytestream::write( void *data, unsigned int length ){
    if(writeIterator_+length > maxSize_){
      maxSize_ += RESIZE_AMOUNT;
      data_ = (DATA_TYPE* )realloc(data_, maxSize_);
    }
    memcpy( &data_[writeIterator_], &data, length );
    writeIterator_+= length;
  }
  
  void Bytestream::read( void *data, unsigned int length ){
    assert(readIterator_+length <= maxSize_);
    memcpy( &data, &data_[readIterator_], length );
    readIterator_ += length;
    assert(readIterator_<=maxSize_);
  }
  
  void Bytestream::readString(std::string& s){
    void readString(std::string& s);
    readIterator_ += s.length()+1;
    assert(readIterator_<=maxSize_);
  }
  
  
