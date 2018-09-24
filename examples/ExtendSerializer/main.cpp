//
// Created by redra on 10.11.17.
//

#include "SuperPuperClass.hpp"
#include <HeapPackBuffer.hpp>

int main() {
  using Buffers::HeapPackBuffer;
  using Buffers::UnpackBuffer;

  HeapPackBuffer buffer(200);
  {
    SuperPuperClass mySuperClass;
    buffer.put(mySuperClass);
  }

  UnpackBuffer unbuffer(buffer.getData(), buffer.getDataSize());
  SuperPuperClass unpackSuperClass = unbuffer.get<SuperPuperClass>();
}