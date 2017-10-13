//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/CyclicBuffer.hpp"

using Buffers::CyclicBuffer;

struct CyclicBufferIntTest : testing::Test
{
  CyclicBuffer<3> * cyclicBuffer;

  virtual void SetUp() {
    cyclicBuffer = new CyclicBuffer<3>;
  };

  virtual void TearDown() {
    delete cyclicBuffer;
  };
};

TEST_F(CyclicBufferIntTest, ValidIntTest0)
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

TEST_F(CyclicBufferIntTest, ValidIntTest1)
{
  cyclicBuffer;
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
  cyclicBuffer->push(new uint8_t{ 1 });
  cyclicBuffer->push(new uint8_t{ 2 });
  cyclicBuffer->push(new uint8_t{ 3 });
  ASSERT_EQ(*cyclicBuffer->pop(), 2);
  ASSERT_EQ(*cyclicBuffer->pop(), 3);
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
}

struct CyclicBufferStringTest : testing::Test
{
  CyclicBuffer<3, std::string> * cyclicBuffer;

  virtual void SetUp() {
    cyclicBuffer = new CyclicBuffer<3, std::string>;
  };

  virtual void TearDown() {
    delete cyclicBuffer;
  };
};

TEST_F(CyclicBufferStringTest, DefaultTypeTest)
{
  cyclicBuffer;
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
  cyclicBuffer->push(new std::string{ "Hi" });
  cyclicBuffer->push(new std::string{ "Hello" });
  cyclicBuffer->push(new std::string{ "Hi vs Hello" });
  ASSERT_NE(cyclicBuffer->pop(), nullptr);
  ASSERT_NE(cyclicBuffer->pop(), nullptr);
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
}

TEST_F(CyclicBufferStringTest, ValidIntTest1)
{
  cyclicBuffer;
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
  cyclicBuffer->push(new std::string{ "Hi" });
  cyclicBuffer->push(new std::string{ "Hello" });
  cyclicBuffer->push(new std::string{ "Hi vs Hello" });
  ASSERT_EQ(*cyclicBuffer->pop(), std::string{ "Hello" });
  ASSERT_EQ(*cyclicBuffer->pop(), std::string{ "Hi vs Hello" });
  ASSERT_EQ(cyclicBuffer->pop(), nullptr);
}