//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/CiclicBuffer.hpp"

using Buffers::CyclicBuffer;

struct CyclicBufferTest : testing::Test
{
  CyclicBuffer<int, 3> * cyclicBuffer;

  virtual void SetUp() {
    cyclicBuffer = new CyclicBuffer<int, 3>;
  };

  virtual void TearDown() {
    delete cyclicBuffer;
  };
};

TEST_F(CyclicBufferTest, ValidTest)
{
  cyclicBuffer;
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
  cyclicBuffer->push(new int{ 1 });
  cyclicBuffer->push(new int{ 2 });
  cyclicBuffer->push(new int{ 3 });
  ASSERT_NE(cyclicBuffer->pop(), nullptr);
  ASSERT_NE(cyclicBuffer->pop(), nullptr);
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
}
