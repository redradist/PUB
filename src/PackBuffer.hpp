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
   private:
    /**
     * Class that is responsible for holding current PackBuffer context:
     *     next position in the message, size of left message space
     */
    class PackBufferContext {
     public:
      friend class PackBuffer;

      PackBufferContext & operator +=(const size_t & _size) {
        p_msg_ += _size;
        size_ -= _size;
        return *this;
      }

      PackBufferContext & operator -=(const size_t & _size) {
        p_msg_ -= _size;
        size_ += _size;
        return *this;
      }

      uint8_t * data() {
        return p_msg_;
      }

      size_t size() {
        return size_;
      }

     private:
      PackBufferContext(uint8_t *& _pMsg, size_t & _size)
        : p_msg_{_pMsg}
        , size_{_size} {
      }

      uint8_t *& p_msg_;
      size_t & size_;
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
    PackBuffer(uint8_t *pMsg, const size_t size)
        : p_msg_(pMsg),
          kSize_(size),
          data_size_(0) {
    }

   public:
    template<typename T>
    bool put(T && t) {
      using GeneralType = typename std::remove_reference<
                            typename std::remove_cv<T>::type
                          >::type;
      auto pMsg = p_msg_ + data_size_;
      auto size = kSize_ - data_size_;
      auto packer = DelegatePackBuffer<GeneralType>{};
      bool result = packer.put(PackBufferContext{pMsg, size}, std::forward<T>(t));
      data_size_ = kSize_ - size;
      return result;
    }

    template <typename T, size_t dataLen>
    bool put(const T t[dataLen]) {
      auto pMsg = p_msg_ + data_size_;
      auto size = kSize_ - data_size_;
      auto packer = DelegatePackBuffer<T>{};
      bool result = packer.put(PackBufferContext{pMsg, size}, t);
      data_size_ = kSize_ - size;
      return result;
    }

    template<typename T>
    bool put(T * t, size_t dataLen) {
      auto pMsg = p_msg_ + data_size_;
      auto size = kSize_ - data_size_;
      auto packer = DelegatePackBuffer<T>{};
      bool result = packer.put(PackBufferContext{pMsg, size}, t, dataLen);
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
    bool put(TBufferContext _ctx, const T & t) {
      bool result = false;
      if (sizeof(T) <= _ctx.size()) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(&t);
        std::copy(p_start_, p_start_ + sizeof(T), _ctx.data());
        _ctx += sizeof(T);
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
    template <typename TBufferContext, size_t dataLen>
    bool put(TBufferContext _ctx, const T t[dataLen]) {
      bool result = false;
      if (t && (sizeof(dataLen) + sizeof(T) * dataLen) <= _ctx.size()) {
        if (DelegatePackBuffer<decltype(dataLen)>{}.put(_ctx, dataLen)) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(t);
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
     * @param t Pointer on first element of packing data
     * @param dataLen Length of data to be stored
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    bool put(TBufferContext _ctx, T * t, size_t dataLen) {
      bool result = false;
      if (t && (sizeof(dataLen) + sizeof(T) * dataLen) <= _ctx.size()) {
        if (DelegatePackBuffer<decltype(dataLen)>{}.put(_ctx, dataLen)) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(t);
          std::copy(p_start_, p_start_ + sizeof(T) * dataLen, _ctx.data());
          _ctx += sizeof(T) * dataLen;
          result = true;
        }
      }
      return result;
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
    bool put(TBufferContext _ctx, const char *str) {
      bool result = false;
      if (str) {
        int kCStringLen = std::strlen(str) + 1;
        if (kCStringLen <= _ctx.size()) {
          const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str);
          std::copy(p_start_, p_start_ + kCStringLen, _ctx.data());
          _ctx += kCStringLen;
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
    template <typename TBufferContext>
    bool put(TBufferContext _ctx, char *str) {
      return this->put(_ctx, static_cast<const char *>(str));
    }
  };

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
    bool put(TBufferContext _ctx, const std::string &str) {
      bool result = false;
      int kCStringLen = str.size() + 1;
      if (kCStringLen <= _ctx.size()) {
        const uint8_t *p_start_ = reinterpret_cast<const uint8_t *>(str.c_str());
        std::copy(p_start_, p_start_ + kCStringLen, _ctx.data());
        _ctx += kCStringLen;
        result = true;
      }
      return result;
    }

    /**
     * Method for packing in buffer lvalue standard string
     * @param str String for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    bool put(TBufferContext _ctx, std::string &str) {
      return this->put(_ctx, static_cast<const std::string &>(str));
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
    bool put(TBufferContext _ctx, const std::vector<T> & vec) {
      bool result = false;
      if (vec.size() > 0) {
        if (DelegatePackBuffer<decltype(vec.size())>{}.put(_ctx, vec.size()) &&
            vec.size() <= _ctx.size()) {
          std::copy(vec.data(), vec.data() + vec.size(), (T*)(_ctx.data()));
          _ctx += vec.size() * sizeof(T);
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
    template <typename TBufferContext>
    bool put(TBufferContext _ctx, std::vector<T> && vec) {
      bool result = false;
      if (vec.size() > 0) {
        if (DelegatePackBuffer<decltype(vec.size())>{}.put(_ctx, vec.size()) &&
            vec.size() <= _ctx.size()) {
          std::move(vec.data(), vec.data() + vec.size(), (T*)(_ctx.data()));
          _ctx += vec.size() * sizeof(T);
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
   public:
    /**
     * Method for packing std::list in buffer
     * @tparam T Type of std::list
     * @param lst std::list for packing
     * @return Return true if packing is succeed, false otherwise
     */
    template <typename TBufferContext>
    bool put(TBufferContext _ctx, const std::list<T> & lst) {
      bool result = false;
      if (lst.size() > 0) {
        if (DelegatePackBuffer<decltype(lst.size())>{}.put(_ctx, lst.size()) &&
            lst.size() <= _ctx.size()) {
          for (auto ve : lst) {
            DelegatePackBuffer<T>{}.put(_ctx, ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported for std::list
     */
    bool put(std::list<T> && lst) = delete;
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
    bool put(TBufferContext _ctx, const std::set<K> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>{}.put(_ctx, mp.size()) &&
            mp.size() <= _ctx.size()) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>{}.put(_ctx, ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported for std::set
     */
    bool put(std::set<K> && mp) = delete;
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
    bool put(TBufferContext _ctx, const std::pair<K, V> & pr) {
      DelegatePackBuffer<K>{}.put(_ctx, pr.first);
      DelegatePackBuffer<V>{}.put(_ctx, pr.second);
      return true;
    }

    /**
     * Move semantic is not supported for std::map
     */
    bool put(std::pair<K, V> && mp) = delete;
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
    bool put(TBufferContext _ctx, const std::map<K, V> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>{}.put(_ctx, mp.size()) &&
            mp.size() <= _ctx.size()) {
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
     * Move semantic is not supported for std::map
     */
    bool put(std::map<K, V> && mp) = delete;
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
    bool put(TBufferContext _ctx, const std::unordered_set<K> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>{}.put(_ctx, mp.size()) &&
            mp.size() <= _ctx.size()) {
          for (auto& ve : mp) {
            DelegatePackBuffer<K>{}.put(_ctx, ve);
          }
          result = true;
        }
      }
      return result;
    }

    /**
     * Move semantic is not supported for std::unordered_set
     */
    bool put(std::unordered_set<K> && mp) = delete;
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
    bool put(TBufferContext _ctx, const std::unordered_map<K, V> & mp) {
      bool result = false;
      if (mp.size() > 0) {
        if (DelegatePackBuffer<decltype(mp.size())>{}.put(_ctx, mp.size()) &&
            mp.size() <= _ctx.size()) {
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
     * Move semantic is not supported for std::unordered_map
     */
    bool put(std::unordered_map<K, V> && mp) = delete;
  };
}

#endif //BUFFERS_PACKBUFFER_HPP