//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/HeapPackBuffer.hpp"
#include "Buffers/UnpackBuffer.hpp"

using Buffers::HeapPackBuffer;
using Buffers::UnpackBuffer;

struct HeapPackBufferTest : testing::Test
{
  HeapPackBuffer * buffer;
  virtual void SetUp() {
    buffer = new HeapPackBuffer(3);
  };

  virtual void TearDown() {
    delete buffer;
  };
};

TEST_F(HeapPackBufferTest, ValidTest)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
}

TEST_F(HeapPackBufferTest, OverflowTest)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 4 }), false);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
}