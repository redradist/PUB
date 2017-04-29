//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/StackPackBuffer.hpp"
#include "Buffers/UnpackBuffer.hpp"

using Buffers::StackPackBuffer;
using Buffers::UnpackBuffer;

struct StackPackBufferTest : testing::Test
{
  StackPackBuffer<3> * buffer;
  virtual void SetUp() {
    buffer = new StackPackBuffer<3>();
  };

  virtual void TearDown() {
    delete buffer;
  };
};

TEST_F(StackPackBufferTest, ValidTest)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
}

TEST_F(StackPackBufferTest, OverflowTest)
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