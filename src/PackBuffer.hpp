/**
 * @file PackBuffer.hpp
 * @author Denis Kotov
 * @date 17 Apr 2017
 * @brief Contains abstract class for Pack Buffer
 * @copyright MIT License. Open source: https://github.com/redradist/Buffers.git
 */

#ifndef BUFFERS_PACKBUFFER_HPP
#define BUFFERS_PACKBUFFER_HPP

#include <stdint.h>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <type_traits>

namespace Buffers {
  /**
   * Pack buffer class
   */
  class PackBuffer {
   public:
    /**
     * Class that is responsible for holding current PackBuffer context:
     *     next position in the message, size of left message space
     * NOTE: This class should be used only by reference in custom PackBuffer
     */
    class Context {
     public:
      friend class PackBuffer;

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

      uint8_t * buffer() const {
        return p_msg_;
      }

      size_t buffer_size() const {
        return (buf_size_ - msg_size_);
      }

     private:
      Context(uint8_t * _pMsg, size_t _size)
        : buf_size_{_size}
        , p_msg_{_pMsg}
        , msg_size_{0} {
      }

      const size_t buf_size_;
      uint8_t * p_msg_;
      size_t msg_size_;
    };

    /**
     * Forward declaration of real delegate pack buffer
     * @tparam T Type for packing
     */
    template <typename T>
    class DelegatePackBuffer;

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
    PackBuffer(uint8_t * const _pMsg, const size_t size)
        : p_buf_(_pMsg)
        , context_(_pMsg, size) {
    }

    /**
     * Delegate constructor for packing buffer.
     * THIS VERSION COULD BE UNSAFE IN CASE OF DUMMY USING !!
     * Better to use main constructor
     * @param _pMsg Pointer to the raw buffer
     */
    PackBuffer(uint8_t * const _pMsg)
        : PackBuffer(_pMsg, std::numeric_limits<size_t>::max()) {
    }

   public:
    template<typename T>
    bool put(const T & _t) {
      using GeneralType = typename std::remove_reference<
                            typename std::remove_cv<T>::type
                          >::type;
      auto packer = DelegatePackBuffer<GeneralType>{};
      bool result = packer.put(context_, _t);
      return result;
    }

    template <typename T, size_t dataLen>
    bool put(const T (&_buffer)[dataLen]) {
      auto packer = DelegatePackBuffer<T>{};
      bool result = packer.put(context_, _buffer);
      return result;
    }

    template <size_t dataLen>
    bool put(const char (&_buffer)[dataLen]);

    template<typename T>
    bool put(const T * _buffer, size_t dataLen) {
      auto packer = DelegatePackBuffer<T>{};
      bool result = packer.put(context_, _buffer, dataLen);
      return result;
    }

    template< typename T >
    static size_t getTypeSize() {
      return DelegatePackBuffer<T>{}.getTypeSize();
    }

    template< typename T >
    static size_t getTypeSize(const T & _t) {
      return DelegatePackBuffer<T>{}.getTypeSize(_t);
    }

    template< typename T, size_t dataLen >
    static size_t getTypeSize(const T _t[dataLen]) {
      return DelegatePackBuffer<T>{}.getTypeSize(_t);
    }

    template< typename T >
    static size_t getTypeSize(const T * _t, size_t dataLen) {
      return DelegatePackBuffer<T>{}.getTypeSize(_t, dataLen);
    }

    /**
     * Method for reset packing data to the buffer
     */
    void reset() {
      context_ -= context_.msg_size_;
    }

    /**
     * Method implicit conversion buffer to the raw pointer
     * @return Raw pointer to the packed data
     */
    operator uint8_t const *() const {
      return p_buf_;
    }

    /**
     * Method for getting raw pointer to packed buffer
     * @return Raw pointer to the packed data
     */
    uint8_t const * getData() const {
      return p_buf_;
    }

    /**
     * Method for getting size of raw pointer to packed buffer
     * @return Size of raw pointer to packed buffer
     */
    size_t getDataSize() const {
      return context_.msg_size_;
    }

    /**
     * Method for getting size of packed buffer
     * @return Size of packed buffer
     */
    size_t getBufferSize() const {
      return context_.buffer_size();
    }

   protected:
    uint8_t * const p_buf_;
    Context context_;
  };

  inline
  PackBuffer::~PackBuffer() {
  }

  /**
   * Class which PackBuffer delegate real unpacking of data for trivial type
   * @tparam T Data to unpack. Should be a trivial type
   */
  template <typename T>
  class PackBuffer::DelegatePackBuffer {
#if __cplusplus > 199711L
    static_assert(std::is_trivial<T>::value, "Type T is not a trivial type !!");
#endif

   public:
    /**
     * Method for packing in buffer constant or temporary data
     * @tparam T Type of packing data
     * @param t Data for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const T & t) {
      bool result = false;
      if (sizeof(T) <= _ctx.buffer_size()) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(&t);
        std::copy(p_start_, p_start_ + sizeof(T), _ctx.buffer());
        _ctx += sizeof(T);
        result = true;
      }
      return result;
    }

    /**
     * Method for packing in buffer array of data
     * @tparam dataLen Array lenght
     * @param _buffer Array to packing data
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext, size_t dataLen>
    static bool put(TBufferContext & _ctx, const T (&_buffer)[dataLen]) {
      bool result = false;
      if (_buffer && (sizeof(dataLen) + sizeof(T) * dataLen) <= _ctx.size()) {
        if (DelegatePackBuffer<decltype(dataLen)>{}.put(_ctx, dataLen)) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(_buffer);
          std::copy(p_start_, p_start_ + sizeof(T) * dataLen, _ctx.data());
          _ctx += sizeof(T) * dataLen;
          result = true;
        }
      }
      return result;
    }


    /**
     * Method for packing in buffer array of data
     * @tparam T Type of packing data
     * @param _buffer Pointer on first element of packing data
     * @param dataLen Length of data to be stored
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const T * _buffer, const size_t dataLen) {
      bool result = false;
      if (_buffer && (sizeof(dataLen) + sizeof(T) * dataLen) <= _ctx.buffer_size()) {
        if (DelegatePackBuffer<decltype(dataLen)>{}.put(_ctx, dataLen)) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(_buffer);
          std::copy(p_start_, p_start_ + sizeof(T) * dataLen, _ctx.buffer());
          _ctx += sizeof(T) * dataLen;
          result = true;
        }
      }
      return result;
    }

    static size_t getTypeSize() {
      return sizeof(T);
    }

    template< size_t dataLen >
    static size_t getTypeSize(const T _buffer[dataLen]) {
      return sizeof(_buffer);
    }

    static size_t getTypeSize(const T * _buffer, const size_t dataLen) {
      return sizeof(size_t) + sizeof(T) * dataLen;
    }
  };

  /**
   * Specialization DelegatePackBuffer class for char*
   */
  template <>
  class PackBuffer::DelegatePackBuffer<char*> {
   public:
    /**
     * Specialization for const null-terminated string
     * @param str Null-terminated string
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const char *str) {
      bool result = false;
      if (str) {
        int kCStringLen = std::strlen(str) + 1;
        if (kCStringLen <= _ctx.buffer_size()) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str);
          std::copy(p_start_, p_start_ + kCStringLen, _ctx.buffer());
          _ctx += kCStringLen;
          result = true;
        }
      }
      return result;
    }

    static size_t getTypeSize(const char *str) {
      return std::strlen(str) + 1;;
    }
  };

  template <size_t dataLen>
  bool PackBuffer::put(const char (&_buffer)[dataLen]) {
    auto packer = DelegatePackBuffer<char *>{};
    bool result = packer.put(context_,
                             static_cast<const char *>(_buffer));
    return result;
  }

  /**
   * Specialization DelegatePackBuffer class for std::string
   */
  template <>
  class PackBuffer::DelegatePackBuffer<std::string> {
   public:
    /**
     * Method for packing in buffer constant or temporary standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::string &str) {
      bool result = false;
      int kCStringLen = str.size() + 1;
      if (kCStringLen <= _ctx.buffer_size()) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str.c_str());
        std::copy(p_start_, p_start_ + kCStringLen, _ctx.buffer());
        _ctx += kCStringLen;
        result = true;
      }
      return result;
    }

    static size_t getTypeSize(const std::string & str) {
      return str.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::vector
   * @tparam T Type of data under std::vector
   */
  template <typename T>
  class PackBuffer::DelegatePackBuffer<std::vector<T>> {
   public:
    /**
     * Method for packing std::vector in buffer
     * @tparam T Type of std::vector
     * @param vec std::vector for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::vector<T> & vec) {
      bool result = false;
      if (vec.size() > 0) {
        if (DelegatePackBuffer<decltype(vec.size())>{}.put(_ctx, vec.size()) &&
            vec.size() <= _ctx.buffer_size()) {
          std::copy(vec.data(), vec.data() + vec.size(), (T*)(_ctx.buffer()));
          _ctx += vec.size() * sizeof(T);
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::vector<T> && lst) = delete;

    static size_t getTypeSize(const std::string & str) {
      return str.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::list
   * @tparam T Type of data under std::list
   */
  template <typename T>
  class PackBuffer::DelegatePackBuffer<std::list<T>> {
   public:
    /**
     * Method for packing std::list in buffer
     * @tparam T Type of std::list
     * @param lst std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::list<T> & lst) {
      bool result = false;
      if (lst.size() > 0) {
        if (DelegatePackBuffer<decltype(lst.size())>{}.put(_ctx, lst.size()) &&
            lst.size() <= _ctx.buffer_size()) {
          for (auto ve : lst) {
            DelegatePackBuffer<T>{}.put(_ctx, ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::list<T> && lst) = delete;

    static size_t getTypeSize(const std::list<T> & lst) {
      return lst.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::set
   * @tparam K Type of data under std::set
   */
  template <typename K>
  class PackBuffer::DelegatePackBuffer<std::set<K>> {
   public:
    /**
     * Method for packing std::set in buffer
     * @tparam K Type of std::set
     * @param mp std::set for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::set<K> & _set) {
      bool result = false;
      if (_set.size() > 0) {
        if (DelegatePackBuffer<decltype(_set.size())>{}.put(_ctx, _set.size()) &&
            _set.size() <= _ctx.buffer_size()) {
          for (auto& ve : _set) {
            DelegatePackBuffer<K>{}.put(_ctx, ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::set<K> && _set) = delete;

    static size_t getTypeSize(const std::set<K> & _set) {
      return _set.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::pair
   * @tparam K First value of std::pair
   * @tparam V Second value of std::pair
   */
  template <typename K, typename V>
  class PackBuffer::DelegatePackBuffer<std::pair<K, V>> {
   public:
    /**
     * Method for packing std::map in buffer
     * @tparam K Key of std::map
     * @tparam V Value of std::map
     * @param mp std::map for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::pair<K, V> & pr) {
      DelegatePackBuffer<K>{}.put(_ctx, pr.first);
      DelegatePackBuffer<V>{}.put(_ctx, pr.second);
      return true;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::pair<K, V> && pr) = delete;

    static size_t getTypeSize(const std::pair<K, V> & pr) {
      return pr.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::map
   * @tparam K Key of std::map
   * @tparam V Value of std::map
   */
  template <typename K, typename V>
  class PackBuffer::DelegatePackBuffer<std::map<K, V>> {
   public:
    /**
     * Method for packing std::map in buffer
     * @tparam K Key of std::map
     * @tparam V Value of std::map
     * @param mp std::map for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::map<K, V> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>{}.put(_ctx, mp.size()) &&
            mp.size() <= _ctx.buffer_size()) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>{}.put(_ctx, ve.first);
            DelegatePackBuffer<V>{}.put(_ctx, ve.second);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::map<K, V> && mp) = delete;

    static size_t getTypeSize(const std::map<K, V> & mp) {
      return mp.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::unordered_set
   * @tparam K Type of data under std::unordered_set
   */
  template <typename K>
  class PackBuffer::DelegatePackBuffer<std::unordered_set<K>> {
   public:
    /**
     * Method for packing std::unordered_set in buffer
     * @tparam K Type of std::unordered_set
     * @param mp std::unordered_set for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::unordered_set<K> & _set) {
      bool result = false;
      if (_set.size() > 0) {
        if (DelegatePackBuffer<decltype(_set.size())>{}.put(_ctx, _set.size()) &&
            _set.size() <= _ctx.buffer_size()) {
          for (auto& ve : _set) {
            DelegatePackBuffer<K>{}.put(_ctx, ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::unordered_set<K> && _set) = delete;

    static size_t getTypeSize(const std::unordered_set<K> & _set) {
      return _set.size();
    }
  };

  /**
   * Specialization DelegatePackBuffer class for std::unordered_map
   * @tparam K Key of std::unordered_map
   * @tparam V Value of std::unordered_map
   */
  template <typename K, typename V>
  class PackBuffer::DelegatePackBuffer<std::unordered_map<K, V>> {
   public:
    /**
     * Method for packing std::unordered_map in buffer
     * @tparam K Key of std::unordered_map
     * @tparam V Value of std::unordered_map
     * @param mp std::unordered_map for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    static bool put(TBufferContext & _ctx, const std::unordered_map<K, V> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>{}.put(_ctx, mp.size()) &&
            mp.size() <= _ctx.buffer_size()) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>{}.put(_ctx, ve.first);
            DelegatePackBuffer<V>{}.put(_ctx, ve.second);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported
     */
    static bool put(std::unordered_map<K, V> && mp) = delete;

    static size_t getTypeSize(const std::unordered_map<K, V> & mp) {
      return mp.size();
    }
  };
}

#endif //BUFFERS_PACKBUFFER_HPP