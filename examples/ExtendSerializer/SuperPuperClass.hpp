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

 private:
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
  uint8_t *& p_msg_;
  size_t & size_;

 public:
  /**
   *
   * @param _type
   * @return
   */
  bool put(const SuperPuperClass & _type) {
    bool result = false;
    if (DelegatePackBuffer<decltype(_type.a)>{p_msg_, size_}.put(_type.a) &&
        DelegatePackBuffer<decltype(_type.k)>{p_msg_, size_}.put(_type.k)) {
      result = true;
    }
    return result;
  }
};

/**
 * Specialization DelegatePackBuffer class for SuperPuperClass
 */
template <>
class UnpackBuffer::DelegateUnpackBuffer<SuperPuperClass> {
 public:
  SuperPuperClass get(uint8_t const *& p_pos_) {
    SuperPuperClass result;
    result.a = DelegateUnpackBuffer<decltype(result.a)>().get(p_pos_);
    result.k = DelegateUnpackBuffer<decltype(result.k)>().get(p_pos_);
    return std::move(result);
  }
};
}

#endif //EXTENDEDSERIALIZER_SUPERPUPERCLASS_HPP
