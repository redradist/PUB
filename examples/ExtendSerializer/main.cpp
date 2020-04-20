//
// Created by redra on 10.11.17.
//

#include <iostream>
#include <pub/HeapPackBuffer.hpp>
#include "SuperPuperClass.hpp"

int main() {
  using buffers::HeapPackBuffer;
  using buffers::UnpackBuffer;

  HeapPackBuffer buffer(200);
  {
    const auto kSize1 = HeapPackBuffer::getTypeSize<int>();
    std::cout << "kSize1 of class SuperPuperClass = " << kSize1 << std::endl;
    const auto kSize = buffer.getTypeSize<SuperPuperClass>();
    std::cout << "Size of class SuperPuperClass = " << kSize << std::endl;
    SuperPuperClass mySuperClass;
    mySuperClass.a = 2;
    buffer << mySuperClass;
//    buffer.put(mySuperClass);
  }

  UnpackBuffer unbuffer(buffer.getData(), buffer.getDataSize());
  SuperPuperClass unpackSuperClass;
  unbuffer >> unpackSuperClass;
//  SuperPuperClass unpackSuperClass = unbuffer.get<SuperPuperClass>();
  std::cout << "unpackSuperClass.a = " << unpackSuperClass.a << std::endl;
}