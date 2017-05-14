/**
 * @file StackPackBuffer.hpp
 * @author Denis Kotov
 * @date 19 Apr 2017
 * @brief Contains library for creating Stack based Pack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Transport_Buffers.git
 */

#ifndef BUFFERS_STACKPACKBUFFER_HPP
#define BUFFERS_STACKPACKBUFFER_HPP

#include <stdint.h>
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