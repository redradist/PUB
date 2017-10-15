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
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

namespace Buffers {
  /**
   * Unpack buffer class
   */
  class UnpackBuffer {
   private:
    /**
     * Class which UnpackBuffer delegate real unpacking of data
     * @tparam T Data to unpack
     */
    template <typename T>
    class DelegateUnpackBuffer {
     public:
      T get(uint8_t const *& p_pos_) {
        const T &t = *(reinterpret_cast<const T *>(p_pos_));
        p_pos_ += sizeof(T);
        return std::move(t);
      }
    };

   public:
    /**
     * Constructor for unpacking buffer
     * @param pMsg Pointer to the raw buffer
     */
    UnpackBuffer(void const *pMsg)
        : p_pos_(reinterpret_cast<uint8_t const *>(pMsg)),
          p_msg_(reinterpret_cast<uint8_t const *>(pMsg)) {
    }

    /**
     * Template getting type T from the buffer
     * @tparam T Type for getting from buffer
     * @return Reference to type T
     */
    template<typename T>
    T get() {
      auto unpacker = DelegateUnpackBuffer<T>();
      return unpacker.get(p_pos_);
    }

    const char *get() {
      return this->get<const char*>();
    }

    /**
     * Method for reset unpacking data from the buffer
     */
    void reset() {
      p_pos_ = p_msg_;
    }

   private:
    uint8_t const *p_pos_;
    uint8_t const *p_msg_;
  };

  /**
   * Specialization for null-terminated string
   * @return Null-terminated string
   */
  template<>
  char *UnpackBuffer::get<char*>() = delete;

  template<>
  class UnpackBuffer::DelegateUnpackBuffer<const char *> {
   public:
    /**
     * Specialization for null-terminated string
     * @return Null-terminated string
     */
    const char *get(uint8_t const *& p_pos_) {
      const char *t = reinterpret_cast<const char *>(p_pos_);
      p_pos_ += std::strlen(t) + 1;
      return t;
    }
  };

  template<>
  class UnpackBuffer::DelegateUnpackBuffer<std::string> {
   public:
    std::string get(uint8_t const *& p_pos_) {
      std::string result = DelegateUnpackBuffer<const char*>().get(p_pos_);
      return std::move(result);
    }
  };

  template<typename T>
  class UnpackBuffer::DelegateUnpackBuffer<std::vector<T>> {
   public:
    std::vector<T> get(uint8_t const *& p_pos_) {
      std::vector<T> result;
      auto size = DelegateUnpackBuffer< typename std::vector<T>::size_type >().get(p_pos_);
      for (int i = 0; i < size; ++i) {
        result.push_back(DelegateUnpackBuffer<T>().get(p_pos_));
      }
      return std::move(result);
    }
  };

  template<typename T>
  class UnpackBuffer::DelegateUnpackBuffer<std::list<T>> {
   public:
    std::list<T> get(uint8_t const *& p_pos_) {
      std::list<T> result;
      auto size = DelegateUnpackBuffer< typename std::vector<T>::size_type >().get(p_pos_);
      for (int i = 0; i < size; ++i) {
        result.push_back(DelegateUnpackBuffer<T>().get(p_pos_));
      }
      return std::move(result);
    }
  };

  template<typename K>
  class UnpackBuffer::DelegateUnpackBuffer<std::set<K>> {
   public:
    std::set<K> get(uint8_t const *& p_pos_) {
      std::set<K> result;
      auto size = DelegateUnpackBuffer< typename std::set<K>::size_type >().get(p_pos_);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>().get(p_pos_);
        result.insert(key);
      }
      return std::move(result);
    }
  };

  template<typename K, typename V>
  class UnpackBuffer::DelegateUnpackBuffer<std::pair<K, V>> {
   public:
    std::pair<K, V> get(uint8_t const *& p_pos_) {
      std::pair<K, V> result;
      result.first = DelegateUnpackBuffer<K>().get(p_pos_);
      result.second = DelegateUnpackBuffer<V>().get(p_pos_);
      return std::move(result);
    }
  };

  template<typename K, typename V>
  class UnpackBuffer::DelegateUnpackBuffer<std::map<K, V>> {
   public:
    std::map<K, V> get(uint8_t const *& p_pos_) {
      std::map<K, V> result;
      auto size = DelegateUnpackBuffer< typename std::map<K, V>::size_type >().get(p_pos_);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>().get(p_pos_);
        auto value = DelegateUnpackBuffer<V>().get(p_pos_);
        result[key] = value;
      }
      return std::move(result);
    }
  };

  template<typename K>
  class UnpackBuffer::DelegateUnpackBuffer<std::unordered_set<K>> {
   public:
    std::unordered_set<K> get(uint8_t const *& p_pos_) {
      std::unordered_set<K> result;
      auto size = DelegateUnpackBuffer< typename std::unordered_set<K>::size_type >().get(p_pos_);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>().get(p_pos_);
        result.insert(key);
      }
      return std::move(result);
    }
  };

  template<typename K, typename V>
  class UnpackBuffer::DelegateUnpackBuffer<std::unordered_map<K, V>> {
   public:
    std::unordered_map<K, V> get(uint8_t const *& p_pos_) {
      std::unordered_map<K, V> result;
      auto size = DelegateUnpackBuffer< typename std::unordered_map<K, V>::size_type >().get(p_pos_);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>().get(p_pos_);
        auto value = DelegateUnpackBuffer<V>().get(p_pos_);
        result[key] = value;
      }
      return std::move(result);
    }
  };
}

#endif //BUFFERS_UNPACKBUFFER_HPP