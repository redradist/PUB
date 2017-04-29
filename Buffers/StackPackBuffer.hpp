//
// Created by redra on 17.04.2017.
//

#ifndef BUFFERS_STACKPACKBUFFER_HPP
#define BUFFERS_STACKPACKBUFFER_HPP

#include <cstdint>
#include "PackBuffer.hpp"

namespace Buffers {
  /*!
   * Pack buffer class based on stack buffer
   * @tparam _Size Size of stack buffer
   */
  template<size_t _Size>
  class StackPackBuffer
      : public PackBuffer {
   public:
    StackPackBuffer()
        : PackBuffer(buffer_, _Size) {
    }

   protected:
    uint8_t buffer_[_Size];
  };
}

#endif //BUFFERS_STACKPACKBUFFER_HPP