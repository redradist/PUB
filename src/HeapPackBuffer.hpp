/**
 * @file HeapPackBuffer.hpp
 * @author Denis Kotov
 * @date 19 Apr 2017
 * @brief Contains library for creating Heap based Pack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Buffers.git
 */

#ifndef BUFFERS_HEAPPACKBUFFER_HPP
#define BUFFERS_HEAPPACKBUFFER_HPP

#include <stdint.h>
#include "PackBuffer.hpp"

namespace Buffers {
/**
 * Pack buffer class based on heap buffer
 * @tparam _Size Size of heap buffer
 */
class HeapPackBuffer
    : public PackBuffer {
 public:
  HeapPackBuffer(const size_t size)
      : PackBuffer(new uint8_t[size]{0}, size) {
  }

  ~HeapPackBuffer() {
    delete [] getData();
  }
};
}

#endif //BUFFERS_HEAPPACKBUFFER_HPP