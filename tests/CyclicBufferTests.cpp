//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/CyclicBuffer.hpp"

using Buffers::CyclicBuffer;

struct CyclicBufferTest0 : testing::Test
{
  CyclicBuffer<3, int> * cyclicBuffer;

  virtual void SetUp() {
    cyclicBuffer = new CyclicBuffer<3, int>;
  };

  virtual void TearDown() {
    delete cyclicBuffer;
  };
};

TEST_F(CyclicBufferTest0, ValidTest)
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

struct CyclicBufferTest1 : testing::Test
{
  CyclicBuffer<3> * cyclicBuffer;

  virtual void SetUp() {
    cyclicBuffer = new CyclicBuffer<3>;
  };

  virtual void TearDown() {
    delete cyclicBuffer;
  };
};

TEST_F(CyclicBufferTest1, DefaultTypeTest)
{
  cyclicBuffer;
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
  cyclicBuffer->push(new uint8_t{ 1 });
  cyclicBuffer->push(new uint8_t{ 2 });
  cyclicBuffer->push(new uint8_t{ 3 });
  ASSERT_NE(cyclicBuffer->pop(), nullptr);
  ASSERT_NE(cyclicBuffer->pop(), nullptr);
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
}
