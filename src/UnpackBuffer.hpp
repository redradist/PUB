/**
 * @file UnpackBuffer.hpp
 * @author Denis Kotov
 * @date 17 Apr 2017
 * @brief Contains abstract class for Unpack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Buffers.git
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
   public:
    /**
     * Class that is responsible for holding current UnpackBuffer context:
     *     next position in the message to unpack
     */
    class UnpackBufferContext {
     public:
      friend class UnpackBuffer;

      UnpackBufferContext & operator +=(const size_t & _size) {
        p_pos_ += _size;
        return *this;
      }

      uint8_t const * data() {
        return p_pos_;
      }

     private:
      UnpackBufferContext(uint8_t const *& _pPos)
          : p_pos_{_pPos} {
      }

      uint8_t const *& p_pos_;
    };

    /**
     * Class which UnpackBuffer delegate real unpacking of data
     * @tparam T Data to unpack
     */
    template <typename T>
    class DelegateUnpackBuffer {
     public:
      template <typename TBufferContext>
      static T get(TBufferContext _ctx) {
        const T &t = *(reinterpret_cast<const T *>(_ctx.data()));
        _ctx += sizeof(T);
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
      auto unpacker = DelegateUnpackBuffer<T>{};
      return unpacker.get(UnpackBufferContext{p_pos_});
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
    template <typename TBufferContext>
    static const char *get(TBufferContext _ctx) {
      const char *t = reinterpret_cast<const char *>(_ctx.data());
      _ctx += std::strlen(t) + 1;
      return t;
    }
  };

  template<>
  class UnpackBuffer::DelegateUnpackBuffer<std::string> {
   public:
    template <typename TBufferContext>
    static std::string get(TBufferContext _ctx) {
      std::string result = DelegateUnpackBuffer<const char*>{}.get(_ctx);
      return std::move(result);
    }
  };

  template<typename T>
  class UnpackBuffer::DelegateUnpackBuffer<std::vector<T>> {
   public:
    template <typename TBufferContext>
    static std::vector<T> get(TBufferContext _ctx) {
      std::vector<T> result;
      auto size = DelegateUnpackBuffer< typename std::vector<T>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        result.push_back(DelegateUnpackBuffer<T>{}.get(_ctx));
      }
      return std::move(result);
    }
  };

  template<typename T>
  class UnpackBuffer::DelegateUnpackBuffer<std::list<T>> {
   public:
    template <typename TBufferContext>
    static std::list<T> get(TBufferContext _ctx) {
      std::list<T> result;
      auto size = DelegateUnpackBuffer< typename std::vector<T>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        result.push_back(DelegateUnpackBuffer<T>{}.get(_ctx));
      }
      return std::move(result);
    }
  };

  template<typename K>
  class UnpackBuffer::DelegateUnpackBuffer<std::set<K>> {
   public:
    template <typename TBufferContext>
    static std::set<K> get(TBufferContext _ctx) {
      std::set<K> result;
      auto size = DelegateUnpackBuffer< typename std::set<K>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>{}.get(_ctx);
        result.insert(key);
      }
      return std::move(result);
    }
  };

  template<typename K, typename V>
  class UnpackBuffer::DelegateUnpackBuffer<std::pair<K, V>> {
   public:
    template <typename TBufferContext>
    static std::pair<K, V> get(TBufferContext _ctx) {
      std::pair<K, V> result;
      result.first = DelegateUnpackBuffer<K>{}.get(_ctx);
      result.second = DelegateUnpackBuffer<V>{}.get(_ctx);
      return std::move(result);
    }
  };

  template<typename K, typename V>
  class UnpackBuffer::DelegateUnpackBuffer<std::map<K, V>> {
   public:
    template <typename TBufferContext>
    static std::map<K, V> get(TBufferContext _ctx) {
      std::map<K, V> result;
      auto size = DelegateUnpackBuffer< typename std::map<K, V>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>{}.get(_ctx);
        auto value = DelegateUnpackBuffer<V>{}.get(_ctx);
        result[key] = value;
      }
      return std::move(result);
    }
  };

  template<typename K>
  class UnpackBuffer::DelegateUnpackBuffer<std::unordered_set<K>> {
   public:
    template <typename TBufferContext>
    static std::unordered_set<K> get(TBufferContext _ctx) {
      std::unordered_set<K> result;
      auto size = DelegateUnpackBuffer< typename std::unordered_set<K>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>{}.get(_ctx);
        result.insert(key);
      }
      return std::move(result);
    }
  };

  template<typename K, typename V>
  class UnpackBuffer::DelegateUnpackBuffer<std::unordered_map<K, V>> {
   public:
    template <typename TBufferContext>
    static std::unordered_map<K, V> get(TBufferContext _ctx) {
      std::unordered_map<K, V> result;
      auto size = DelegateUnpackBuffer< typename std::unordered_map<K, V>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>().get(_ctx);
        auto value = DelegateUnpackBuffer<V>().get(_ctx);
        result[key] = value;
      }
      return std::move(result);
    }
  };
}

#endif //BUFFERS_UNPACKBUFFER_HPP