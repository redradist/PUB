/**
 * @file PackBuffer.hpp
 * @author Denis Kotov
 * @date 17 Apr 2017
 * @brief Contains abstract class for Pack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Transport_Buffers.git
 */

#ifndef BUFFERS_PACKBUFFER_HPP
#define BUFFERS_PACKBUFFER_HPP

#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

namespace Buffers {
  /**
   * Pack buffer class
   */
  class PackBuffer {
   public:
    /**
     * Destructor for deletion memory if it was allocated by purselves
     */
    virtual ~PackBuffer() = 0;

   protected:
  /**
   * Constructor in which should be put prepared buffer.
   * DO NOT DELETE MEMORY BY YOUSELF INSIDE OF THIS CLASS !!
   * @param pMsg Pointer to the buffer
   * @param size Size of the buffer
   */
    PackBuffer(uint8_t *pMsg, const size_t size)
        : p_msg_(pMsg),
          kSize_(size),
          data_size_(0) {
    }

   public:
    /**
     * Method for packing in buffer constant or temporary data
     * @tparam T Type of packing data
     * @param t Data for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template<typename T>
    bool put(T const &t) {
      bool result = false;
      if ((data_size_ + sizeof(T)) <= kSize_) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(&t);
        std::copy(p_start_, p_start_ + sizeof(T), p_msg_ + data_size_);
        data_size_ += sizeof(T);
        result = true;
      }
      return result;
    }

    /**
     * Method for packing in buffer lvalue data
     * @tparam T Type of packing data
     * @param t Data for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template<typename T>
    bool put(T &t) {
      return put(static_cast<T const &>(t));
    }

    /**
     * Method for packing std::vector in buffer
     * @tparam T Type of std::vector
     * @param vec std::vector for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename T>
    bool put(const std::vector<T> & vec) {
      bool result = false;
      if (vec.size() > 0) {
          if (this->put(vec.size()) && (data_size_ + vec.size() <= kSize_)) {
              std::copy(vec.data(), vec.data() + vec.size(), (T*)(p_msg_+data_size_));
              data_size_ += vec.size() * sizeof(T);
              result = true;
          }
      }
      return result;
    }

    /**
     * Method for packing rvalue std::vector in buffer
     * @tparam T Type of std::vector
     * @param vec rvalue std::vector for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename T>
    bool put(std::vector<T> && vec) {
      bool result = false;
      if (vec.size() > 0) {
          if (this->put(vec.size()) && (data_size_ + vec.size() <= kSize_)) {
              std::move(vec.data(), vec.data() + vec.size(), (T*)(p_msg_+data_size_));
              data_size_ += vec.size() * sizeof(T);
              result = true;
          }
      }
      return result;
    }

    /**
     * Method for packing std::list in buffer
     * @tparam T Type of std::list
     * @param lst std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename T>
    bool put(const std::list<T> & lst) {
      bool result = false;
      if (lst.size() > 0) {
        if (this->put(lst.size()) && (data_size_ + lst.size() <= kSize_)) {
          for (auto ve : lst) {
            this->put(ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Method for packing rvalue std::list in buffer
     * @tparam T Type of std::list
     * @param lst rvalue std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename T>
    bool put(std::list<T> && lst) {
      bool result = false;
      if (lst.size() > 0) {
          if (this->put(lst.size()) && (data_size_ + lst.size() <= kSize_)) {
              for (auto ve : lst) {
                this->put(ve);
              }
              result = true;
          }
      }
      return result;
    }

    /**
     * Method for packing in buffer array of data
     * @tparam T Type of packing data
     * @param t Pointer on first element of packing data
     * @param data_len Length of data to be stored
     * @return Return true if packing is succeed, false otherwise
     */
    template<typename T>
    bool put(T *t, size_t data_len) {
      bool result = false;
      if (t && (data_size_ + sizeof(data_len) + sizeof(T) * data_len) <= kSize_) {
        if (put<size_t>(data_len)) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(t);
          std::copy(p_start_, p_start_ + sizeof(T) * data_len, p_msg_ + data_size_);
          data_size_ += sizeof(T) * data_len;
          result = true;
        }
      }
      return result;
    }

    /**
     * Method for packing in buffer constant or temporary standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::string &str) {
      bool result = false;
      int kCStringLen = str.size() + 1;
      if ((data_size_ + kCStringLen) <= kSize_) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str.c_str());
        std::copy(p_start_, p_start_ + kCStringLen, p_msg_ + data_size_);
        data_size_ += kCStringLen;
        result = true;
      }
      return result;
    }

    /**
     * Method for packing in buffer lvalue standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(std::string &str) {
      return put(static_cast<const std::string &>(str));
    }

    /**
     * Specialization for const null-terminated string
     * @param str Null-terminated string
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const char *str) {
      bool result = false;
      if (str) {
        int kCStringLen = std::strlen(str) + 1;
        if ((data_size_ + kCStringLen) <= kSize_) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str);
          std::copy(p_start_, p_start_ + kCStringLen, p_msg_ + data_size_);
          data_size_ += kCStringLen;
          result = true;
        }
      }
      return result;
    }

    /**
     * Specialization for null-terminated string
     * @param str Null-terminated string
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(char *str) {
      return put(static_cast<const char *>(str));
    }

    /**
     * Method for reset packing data to the buffer
     */
    void reset() {
      data_size_ = 0;
    }

    /**
     * Method for getting raw pointer to packed buffer
     * @return Raw pointer to the packed data
     */
    uint8_t const *getData() const {
      return p_msg_;
    }

    /**
     * Method for getting size of raw pointer to packed buffer
     * @return Size of raw pointer to packed buffer
     */
    size_t getDataSize() const {
      return data_size_;
    }

    /**
     * Method implicit conversion buffer to the raw pointer
     * @return Raw pointer to the packed data
     */
    operator uint8_t const *() const {
      return p_msg_;
    }

    /**
     * Method for getting size of packed buffer
     * @return Size of packed buffer
     */
    size_t getSize() const {
      return kSize_;
    }

   protected:
    uint8_t *p_msg_;
    const size_t kSize_;

    size_t data_size_;
  };

  inline
  PackBuffer::~PackBuffer() {
  }
}

#endif //BUFFERS_PACKBUFFER_HPP