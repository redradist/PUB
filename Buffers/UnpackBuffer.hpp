/**
 * @file UnpackBuffer.hpp
 * @author Denis Kotov
 * @date 17 Apr 2017
 * @brief Contains abstract class for Unpack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Transport_Buffers.git
 */

#ifndef BUFFERS_UNPACKBUFFER_HPP
#define BUFFERS_UNPACKBUFFER_HPP

#include <stdint.h>
#include <cstring>

namespace Buffers {
  /*!
   * Unpack buffer class
   */
  class UnpackBuffer {
   public:
    /*!
     * Constructor for unpacking buffer
     * @param pMsg Pointer to the raw buffer
     */
    UnpackBuffer(void const *pMsg)
        : p_pos_(reinterpret_cast<uint8_t const *>(pMsg)),
          p_msg_(reinterpret_cast<uint8_t const *>(pMsg)) {
    }

    /*!
     * Template getting type T from the buffer
     * @tparam T Type for getting from buffer
     * @return Reference to type T
     */
    template<typename T>
    const T &get() {
      const T &t = *(reinterpret_cast<const T *>(p_pos_));
      p_pos_ += sizeof(T);
      return t;
    }

    /*!
     * Specialization for null-terminated string
     * @return Null-terminated string
     */
    const char *get() {
      const char *t = reinterpret_cast<const char *>(p_pos_);
      p_pos_ += std::strlen(t) + 1;
      return t;
    }

    /*!
     * Method for reset unpacking data from the buffer
     */
    void reset() {
      p_pos_ = p_msg_;
    }

   private:
    uint8_t const *p_pos_;
    uint8_t const *p_msg_;
  };
}

#endif //BUFFERS_UNPACKBUFFER_HPP