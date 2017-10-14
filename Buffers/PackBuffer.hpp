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
   private:
    /**
     * Class which PackBuffer delegate real unpacking of data
     * @tparam T Data to unpack
     */
    template <typename T>
    class DelegatePackBuffer {
     private:
      uint8_t *& p_msg_;
      size_t & size_;

     public:
      DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
          : p_msg_(pMsg),
            size_(size) {
      }

      /**
       * Method for packing in buffer constant or temporary data
       * @tparam T Type of packing data
       * @param t Data for packing
       * @return Return true if packing is succeed, false otherwise
       */
      bool put(const T & t) {
        bool result = false;
        if (sizeof(T) <= size_) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(&t);
          std::copy(p_start_, p_start_ + sizeof(T), p_msg_);
          size_t writtenSize = sizeof(T);
          p_msg_ += writtenSize;
          size_ -= writtenSize;
          result = true;
        }
        return result;
      }

      /**
       * Method for packing in buffer array of data
       * @tparam dataLen Array lenght
       * @param t Array to packing data
       * @return Return true if packing is succeed, false otherwise
       */
      template <size_t dataLen>
      bool put(const T t[dataLen]) {
        bool result = false;
        if (t && (sizeof(dataLen) + sizeof(T) * dataLen) <= size_) {
          if (this->put(dataLen)) {
            const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(t);
            std::copy(p_start_, p_start_ + sizeof(T) * dataLen, p_msg_);
            size_t writtenSize = sizeof(T) * dataLen;
            p_msg_ += writtenSize;
            size_ -= writtenSize;
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
      bool put(T * t, size_t dataLen) {
        bool result = false;
        if (t && (sizeof(dataLen) + sizeof(T) * dataLen) <= size_) {
          if (this->put(dataLen)) {
            const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(t);
            std::copy(p_start_, p_start_ + sizeof(T) * dataLen, p_msg_);
            size_t writtenSize = sizeof(T) * dataLen;
            p_msg_ += writtenSize;
            size_ -= writtenSize;
            result = true;
          }
        }
        return result;
      }
    };

   public:
    /**
     * Destructor for deletion memory if it was allocated by purselves
     */
    virtual ~PackBuffer() = 0;

   protected:
  /**
   * Constructor in which should be put prepared buffer.
   * DO NOT DELETE MEMORY BY YOURSELF INSIDE OF THIS CLASS !!
   * @param pMsg Pointer to the buffer
   * @param size Size of the buffer
   */
    PackBuffer(uint8_t *pMsg, const size_t size)
        : p_msg_(pMsg),
          kSize_(size),
          data_size_(0) {
    }

   public:
    template<typename T>
    bool put(T && t) {
      auto pMsg = p_msg_ + data_size_;
      auto size = kSize_ - data_size_;
      auto packer = DelegatePackBuffer<
          typename std::remove_reference<
            typename std::remove_cv<T>::type
                                        >::type
                                      >(pMsg, size);
      bool result = packer.put(std::forward<T>(t));
      data_size_ = kSize_ - size;
      return result;
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

  /**
   * Specialization DelegatePackBuffer class for char*
   */
  template <>
  class PackBuffer::DelegatePackBuffer<char*> {
   private:
    uint8_t *& p_msg_;
    size_t & size_;

   public:
    DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
        : p_msg_(pMsg),
          size_(size) {
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
        if (kCStringLen <= size_) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str);
          std::copy(p_start_, p_start_ + kCStringLen, p_msg_);
          size_ -= kCStringLen;
          p_msg_ += kCStringLen;
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
      return this->put(static_cast<const char *>(str));
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::string
   */
  template <>
  class PackBuffer::DelegatePackBuffer<std::string> {
   private:
    uint8_t *& p_msg_;
    size_t & size_;

   public:
    DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
        : p_msg_(pMsg),
          size_(size) {
    }

    /**
     * Method for packing in buffer constant or temporary standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::string &str) {
      bool result = false;
      int kCStringLen = str.size() + 1;
      if (kCStringLen <= size_) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str.c_str());
        std::copy(p_start_, p_start_ + kCStringLen, p_msg_);
        size_ -= kCStringLen;
        p_msg_ += kCStringLen;
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
  };

  /**
   * Specialization DelegatePackBuffer class for std::vector
   * @tparam T Type of data under std::vector
   */
  template <typename T>
  class PackBuffer::DelegatePackBuffer<std::vector<T>> {
   private:
    uint8_t *& p_msg_;
    size_t & size_;

   public:
    DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
        : p_msg_(pMsg),
          size_(size) {
    }

    /**
     * Method for packing std::vector in buffer
     * @tparam T Type of std::vector
     * @param vec std::vector for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::vector<T> & vec) {
      bool result = false;
      if (vec.size() > 0) {
        if (DelegatePackBuffer<decltype(vec.size())>(p_msg_, size_).put(vec.size()) &&
            vec.size() <= size_) {
          std::copy(vec.data(), vec.data() + vec.size(), (T*)(p_msg_));
          size_ -= vec.size() * sizeof(T);
          p_msg_ += vec.size() * sizeof(T);
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
    bool put(std::vector<T> && vec) {
      bool result = false;
      if (vec.size() > 0) {
        if (DelegatePackBuffer<decltype(vec.size())>(p_msg_, size_).put(vec.size()) &&
            vec.size() <= size_) {
          std::move(vec.data(), vec.data() + vec.size(), (T*)(p_msg_));
          size_t writtenSize = vec.size() * sizeof(T);
          p_msg_ += writtenSize;
          size_ -= writtenSize;
          result = true;
        }
      }
      return result;
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::list
   * @tparam T Type of data under std::list
   */
  template <typename T>
  class PackBuffer::DelegatePackBuffer<std::list<T>> {
   private:
    uint8_t *& p_msg_;
    size_t & size_;

   public:
    DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
        : p_msg_(pMsg),
          size_(size) {
    }

    /**
     * Method for packing std::list in buffer
     * @tparam T Type of std::list
     * @param lst std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::list<T> & lst) {
      bool result = false;
      if (lst.size() > 0) {
        if (DelegatePackBuffer<decltype(lst.size())>(p_msg_, size_).put(lst.size()) &&
            lst.size() <= size_) {
          for (auto ve : lst) {
            DelegatePackBuffer<T>(p_msg_, size_).put(ve);
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
    bool put(std::list<T> && lst) {
      bool result = false;
      if (lst.size() > 0) {
        if (DelegatePackBuffer<decltype(lst.size())>(p_msg_, size_).put(lst.size()) &&
            lst.size() <= size_) {
          for (auto ve : lst) {
            DelegatePackBuffer<T>(p_msg_, size_).put(ve);
          }
          result = true;
        }
      }
      return result;
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::list
   * @tparam T Type of data under std::list
   */
  template <typename K>
  class PackBuffer::DelegatePackBuffer<std::set<K>> {
   private:
    uint8_t *& p_msg_;
    size_t & size_;

   public:
    DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
        : p_msg_(pMsg),
          size_(size) {
    }

    /**
     * Method for packing std::list in buffer
     * @tparam T Type of std::list
     * @param lst std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::set<K> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>(p_msg_, size_).put(mp.size()) &&
            mp.size() <= size_) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>(p_msg_, size_).put(ve);
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
    bool put(std::set<K> && mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>(p_msg_, size_).put(mp.size()) &&
            mp.size() <= size_) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>(p_msg_, size_).put(ve);
          }
          result = true;
        }
      }
      return result;
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::list
   * @tparam T Type of data under std::list
   */
  template <typename K, typename V>
  class PackBuffer::DelegatePackBuffer<std::map<K, V>> {
   private:
    uint8_t *& p_msg_;
    size_t & size_;

   public:
    DelegatePackBuffer(uint8_t *& pMsg, size_t & size)
        : p_msg_(pMsg),
          size_(size) {
    }

    /**
     * Method for packing std::list in buffer
     * @tparam T Type of std::list
     * @param lst std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    bool put(const std::map<K, V> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>(p_msg_, size_).put(mp.size()) &&
            mp.size() <= size_) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>(p_msg_, size_).put(ve.first);
            DelegatePackBuffer<V>(p_msg_, size_).put(ve.second);
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
    bool put(std::map<K, V> && mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>(p_msg_, size_).put(mp.size()) &&
            mp.size() <= size_) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>(p_msg_, size_).put(ve.first);
            DelegatePackBuffer<V>(p_msg_, size_).put(ve.second);
          }
          result = true;
        }
      }
      return result;
    }
  };
}

#endif //BUFFERS_PACKBUFFER_HPP