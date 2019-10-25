//
// Created by redra on 11.11.17.
//

#ifndef EXTENDEDSERIALIZER_SUPERPUPERCLASS_HPP
#define EXTENDEDSERIALIZER_SUPERPUPERCLASS_HPP

#include <PackBuffer.hpp>
#include <UnpackBuffer.hpp>

class SuperPuperClass {
 public:
  void Run();

  int a = 0;
  double k = 0;
  friend class Buffers::PackBuffer::DelegatePackBuffer<SuperPuperClass>;
  friend class Buffers::UnpackBuffer::DelegateUnpackBuffer<SuperPuperClass>;
};

namespace Buffers {
/**
 * Specialization DelegatePackBuffer class for SuperPuperClass
 */
template <>
class PackBuffer::DelegatePackBuffer<SuperPuperClass> {
 public:
  /**
   * Method for serialization of user-defined data
   * @tparam TBufferContext Class that represent current context of PackBuffer
   * @param _ctx Instance of PackBuffer context
   * @param _type Object to put in PackBuffer
   * @return true if successfully, false - otherwise
   */
  template <typename TBufferContext>
  static bool put(TBufferContext & _ctx, const SuperPuperClass & _type) {
    bool result = false;
    if (DelegatePackBuffer<decltype(_type.a)>::put(_ctx, _type.a) &&
        DelegatePackBuffer<decltype(_type.k)>::put(_ctx, _type.k)) {
      result = true;
    }
    return result;
  }

  static size_t getTypeSize() {
    return 30;
  }
};

/**
 * Specialization DelegatePackBuffer class for SuperPuperClass
 */
template <>
class UnpackBuffer::DelegateUnpackBuffer<SuperPuperClass> {
 public:
  /**
   * Method for deserialization of user-defined data
   * @tparam TBufferContext Class that represent current context of UnpackBuffer
   * @param _ctx Instance of UnpackBuffer context
   * @return Instance of deserialized class
   */
  template <typename TBufferContext>
  static SuperPuperClass get(TBufferContext & _ctx) {
    SuperPuperClass result;
    result.a = DelegateUnpackBuffer<decltype(result.a)>::get(_ctx);
    result.k = DelegateUnpackBuffer<decltype(result.k)>::get(_ctx);
    return std::move(result);
  }
};
}

#endif //EXTENDEDSERIALIZER_SUPERPUPERCLASS_HPP
