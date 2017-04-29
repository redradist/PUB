//
// Created by redra on 17.04.2017.
//

#ifndef BUFFERS_PACKBUFFER_HPP
#define BUFFERS_PACKBUFFER_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <algorithm>

namespace Buffers {
  /*!
   * Pack buffer class
   */
  class PackBuffer {
   public:
    /*!
     * Destructor for deletion memory if it was allocated by purselves
     */
    virtual ~PackBuffer() = 0;

   protected:
  /*!
   * Constructor in which should be put prepared buffer.
   * DO NOT DELETE MEMORY BY YOUSELF !!
   * @param pMsg Pointer to the buffer
   * @param size Size of the buffer
   */
    PackBuffer(uint8_t *pMsg, const size_t size)
        : p_msg_(pMsg),
          kSize_(size),
          pos_(0) {
    }

   public:
    /*!
     * Method for packing in buffer constant or temporary data
     * @tparam T Type of packing data
     * @param t Data for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template<typename T>
    bool put(T const &t) {
      bool result = false;
      if ((pos_ + sizeof(T)) <= kSize_) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(&t);
        std::copy(p_start_, p_start_ + sizeof(T), p_msg_ + pos_);
        pos_ += sizeof(T);
        result = true;
      }
      return result;
    }

    /*!
     * Method for packing in buffer lvalue data
     * @tparam T Type of packing data
     * @param t Data for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template<typename T>
    bool put(T &t) {
      return put(static_cast<T const &>(t));
    }

    /*!
     * Method for packing in buffer array of data
     * @tparam T Type of packing data
     * @param t Pointer on first element of packing data
     * @param data_len Length of data to be stored
     * @return Return true if packing is succeed, false otherwise
     */
    template<typename T>
    bool put(T *t, size_t data_len) {
      bool result = false;
      if ((pos_ + sizeof(data_len) + sizeof(T) * data_len) <= kSize_) {
        if (put<size_t>(data_len)) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(t);
          std::copy(p_start_, p_start_ + sizeof(T) * data_len, p_msg_ + pos_);
          pos_ += sizeof(T) * data_len;
          result = true;
        }
      }
      return result;
    }

    /*!
     * Method for packing in buffer constant or temporary standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::string &str) {
      bool result = false;
      int kCStringLen = str.size() + 1;
      if ((pos_ + kCStringLen) <= kSize_) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str.c_str());
        std::copy(p_start_, p_start_ + kCStringLen, p_msg_ + pos_);
        pos_ += kCStringLen;
        result = true;
      }
      return true;
    }

    /*!
     * Method for packing in buffer lvalue standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(std::string &str) {
      return put(static_cast<const std::string &>(str));
    }

    /*!
     * Specialization for const null-terminated string
     * @param str Null-terminated string
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const char *str) {
      bool result = false;
      int kCStringLen = std::strlen(str) + 1;
      if ((pos_ + kCStringLen) <= kSize_) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str);
        std::copy(p_start_, p_start_ + kCStringLen, p_msg_ + pos_);
        pos_ += kCStringLen;
        result = true;
      }
      return true;
    }

    /*!
     * Specialization for null-terminated string
     * @param str Null-terminated string
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(char *str) {
      return put(static_cast<const char *>(str));
    }

    /*!
     * Method for reset packing data to the buffer
     */
    void reset() {
      pos_ = 0;
    }

    /*!
     * Method for getting raw pointer to packed buffer
     * @return Raw pointer to the packed data
     */
    uint8_t const *getData() const {
      return p_msg_;
    }

    /*!
     * Method implicit conversion buffer to the raw pointer
     * @return Raw pointer to the packed data
     */
    operator uint8_t const *() const {
      return p_msg_;
    }

    /*!
     * Method for getting size of raw pointer to packed buffer
     * @return Size of raw pointer to packed buffer
     */
    size_t getSize() const {
      return pos_;
    }

   protected:
    uint8_t *p_msg_;
    const size_t kSize_;

    size_t pos_;
  };

  inline
  PackBuffer::~PackBuffer() {
  }
}

#endif //BUFFERS_PACKBUFFER_HPP