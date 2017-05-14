/**
 * @file CyclicBuffer.hpp
 * @author Denis Kotov
 * @date 19 Apr 2017
 * @brief Contains library for creating Cyclic Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Transport_Buffers.git
 */

#ifndef BUFFERS_CYCLICBUFFER_HPP
#define BUFFERS_CYCLICBUFFER_HPP

#include <cassert>
#include <algorithm>

namespace Buffers {
  /*!
   * Cyclic buffer for writing and reading data
   * @tparam _Size Size of cyclic buffer
   * @tparam _Type Data type for storing in cyclic buffer
   */
  template <int _Size, typename _Type = uint8_t>
  class CyclicBuffer {
  #if __cplusplus > 199711L
    static_assert(_Size > 0, "_Size should be more than 0");
  #endif
   public:
    CyclicBuffer()
        : head(0)
        , tail(0) {
  #if __cplusplus <= 199711L
      std::assert(_Size > 0 && "_Size should be more than 0");
  #endif
    }

    /*!
     * Method used to push type in buffer
     * @param message Message that need to be put in buffer
     */
    void push(_Type const * message) {
      if (NULL != message) {
        std::copy(message, message+1, &buffer[tail]);
        ++tail;
        if (tail >= _Size) {
          tail = 0;
        }
        if (tail == head) {
          ++head;
          if (head >= _Size) {
            head = 0;
          }
        }
      }
    }

    /*!
     * Method is used to retrieve message from the buffer
     * @return Pointer to the message
     */
    _Type const * pop() {
      _Type * message = NULL;
      if (head != tail) {
        message = &buffer[head];
        ++head;
        if (head >= _Size) {
          head = 0;
        }
      }
      return message;
    }

   private:
    _Type buffer[_Size];

    int head;
    int tail;
  };
}

#endif //BUFFERS_CYCLICBUFFER_HPP