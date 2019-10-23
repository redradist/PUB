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
#include <limits>
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
    class Context {
     public:
      friend class UnpackBuffer;

      Context(const Context&) = delete;
      Context(Context&&) = delete;
      Context& operator=(const Context&) = delete;
      Context& operator=(Context&&) = delete;

      Context & operator +=(const size_t & _size) {
      #ifdef __EXCEPTIONS
        if (buf_size_ < (msg_size_ + _size)) {
          throw std::out_of_range("Acquire more memory than is available !!");
        }
      #endif

        p_msg_ += _size;
        msg_size_ += _size;
        return *this;
      }

      Context & operator -=(const size_t & _size) {
      #ifdef __EXCEPTIONS
        if (msg_size_ < _size) {
          throw std::out_of_range("Release more memory than was originally !!");
        }
      #endif

        p_msg_ -= _size;
        msg_size_ -= _size;
        return *this;
      }

      uint8_t const * buffer() const {
        return p_msg_;
      }

      size_t buffer_size() const {
        return (buf_size_ - msg_size_);
      }

     private:
      Context(uint8_t const * _pMsg, size_t _size)
          : buf_size_{_size}
          , p_msg_{_pMsg}
          , msg_size_{0} {
      }

      const size_t buf_size_;
      uint8_t const * p_msg_;
      size_t msg_size_;
    };

    /**
     * Class which UnpackBuffer delegate real unpacking of data
     * @tparam T Data to unpack
     */
    template <typename T>
    class DelegateUnpackBuffer {
     public:
      template <typename TBufferContext>
      static T get(TBufferContext & _ctx) {
        const T &t = *(reinterpret_cast<const T *>(_ctx.buffer()));
        _ctx += sizeof(T);
        return std::move(t);
      }
    };

   public:
    /**
     * Constructor for unpacking buffer
     * @param _pMsg Pointer to the raw buffer
     * @param _size Size of raw buffer
     */
    UnpackBuffer(uint8_t const * const _pMsg, const size_t _size)
        : p_buf_(_pMsg)
        , context_(_pMsg, _size) {
    }

    /**
     * Delegate constructor for unpacking buffer.
     * THIS VERSION COULD BE UNSAFE IN CASE OF DUMMY USING !!
     * Better to use main constructor
     * @param _pMsg Pointer to the raw buffer
     */
    UnpackBuffer(uint8_t const * const _pMsg)
        : UnpackBuffer(_pMsg, std::numeric_limits<size_t>::max()) {
    }

    /**
     * Constructor for unpacking buffer
     * @param pMsg Pointer to the raw buffer
     */
    template <typename T, size_t dataLen>
    UnpackBuffer(const T (&_buffer)[dataLen])
        : p_buf_(reinterpret_cast<uint8_t const *>(_buffer))
        , context_(p_buf_, sizeof(T) * dataLen) {
    }

    /**
     * Template getting type T from the buffer
     * @tparam T Type for getting from buffer
     * @return Reference to type T
     */
    template<typename T>
    T get() {
      auto unpacker = DelegateUnpackBuffer<T>{};
      T result = unpacker.get(context_);
      return std::move(result);
    }

    const char *get() {
      return this->get<const char*>();
    }

    /**
     * Method for reset unpacking data from the buffer
     */
    void reset() {
      context_ -= context_.msg_size_;
    }

   private:
    const uint8_t * const p_buf_;
    Context context_;
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
    static const char *get(TBufferContext & _ctx) {
      const char *t = reinterpret_cast<const char *>(_ctx.buffer());
      _ctx += std::strlen(t) + 1;
      return t;
    }
  };

  template<>
  class UnpackBuffer::DelegateUnpackBuffer<std::string> {
   public:
    template <typename TBufferContext>
    static std::string get(TBufferContext & _ctx) {
      std::string result = DelegateUnpackBuffer<const char*>{}.get(_ctx);
      return std::move(result);
    }
  };

  template<typename T>
  class UnpackBuffer::DelegateUnpackBuffer<std::vector<T>> {
   public:
    template <typename TBufferContext>
    static std::vector<T> get(TBufferContext & _ctx) {
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
    static std::list<T> get(TBufferContext & _ctx) {
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
    static std::set<K> get(TBufferContext & _ctx) {
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
    static std::pair<K, V> get(TBufferContext & _ctx) {
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
    static std::map<K, V> get(TBufferContext & _ctx) {
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
    static std::unordered_set<K> get(TBufferContext & _ctx) {
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
    static std::unordered_map<K, V> get(TBufferContext & _ctx) {
      std::unordered_map<K, V> result;
      auto size = DelegateUnpackBuffer< typename std::unordered_map<K, V>::size_type >{}.get(_ctx);
      for (int i = 0; i < size; ++i) {
        auto key = DelegateUnpackBuffer<K>{}.get(_ctx);
        auto value = DelegateUnpackBuffer<V>{}.get(_ctx);
        result[key] = value;
      }
      return std::move(result);
    }
  };
}

#endif //BUFFERS_UNPACKBUFFER_HPP