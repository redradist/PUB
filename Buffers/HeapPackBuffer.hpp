//
// Created by redra on 19.04.2017.
//

#ifndef BUFFERS_HEAPPACKBUFFER_HPP
#define BUFFERS_HEAPPACKBUFFER_HPP

#include <cstdint>
#include "PackBuffer.hpp"

namespace Buffers {
/*!
 * Pack buffer class based on stack buffer
 * @tparam _Size Size of stack buffer
 */
class HeapPackBuffer
    : public PackBuffer {
 public:
  HeapPackBuffer(const size_t size)
      : PackBuffer(new uint8_t[size]{0}, size) {
  }

  ~HeapPackBuffer() {
    delete [] p_msg_;
  }
};
}

#endif //BUFFERS_HEAPPACKBUFFER_HPP