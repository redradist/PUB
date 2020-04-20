/**
 * @file StackPackBuffer.hpp
 * @author Denis Kotov
 * @date 19 Apr 2017
 * @brief Contains library for creating Stack based Pack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/PUB.git
 */

#ifndef BUFFERS_STACKPACKBUFFER_HPP
#define BUFFERS_STACKPACKBUFFER_HPP

#include <stdint.h>
#include "PackBuffer.hpp"

namespace buffers {
  /**
   * Pack buffer class based on stack buffer
   * @tparam _Size Size of stack buffer
   */
  template<size_t _Size>
  class StackPackBuffer
      : public PackBuffer {
#if __cplusplus > 199711L
    static_assert(_Size > 0, "_Size should be more than 0");
#endif
   public:
    StackPackBuffer()
        : PackBuffer(buffer_, _Size) {
    }

   protected:
    uint8_t buffer_[_Size];
  };
}

#endif //BUFFERS_STACKPACKBUFFER_HPP