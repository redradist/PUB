//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/StackPackBuffer.hpp"
#include "Buffers/UnpackBuffer.hpp"

using Buffers::StackPackBuffer;
using Buffers::UnpackBuffer;

struct StackPackBufferIntTest : testing::Test
{
  StackPackBuffer<3> * buffer;
  virtual void SetUp() {
    buffer = new StackPackBuffer<3>();
  };

  virtual void TearDown() {
    delete buffer;
  };
};

struct StackPackBufferStringTest : testing::Test
{
  StackPackBuffer<30> * buffer;
  virtual void SetUp() {
    buffer = new StackPackBuffer<30>();
  };

  virtual void TearDown() {
    delete buffer;
  };
};

TEST_F(StackPackBufferIntTest, ValidIntTest)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
}

TEST_F(StackPackBufferIntTest, OverflowIntTest)
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

TEST_F(StackPackBufferIntTest, CrashIntTest0)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(nullptr), false);
  ASSERT_EQ(buffer->put(uint8_t{ 4 }), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 4);
}

TEST_F(StackPackBufferIntTest, CrashIntTest1)
{
  ASSERT_EQ(buffer->put(nullptr), false);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 4 }), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 4);
}

TEST_F(StackPackBufferIntTest, CrashIntTest2)
{
  ASSERT_EQ(buffer->put(nullptr), false);
  ASSERT_EQ(buffer->put(nullptr), false);
}

TEST_F(StackPackBufferStringTest, ValidStringTest0)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hello"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(StackPackBufferStringTest, ValidStringTest1)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(StackPackBufferStringTest, OverflowStringTest)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hello"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi not vs Hello but vs Hi"}), false);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(StackPackBufferStringTest, ValidCStringTest0)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(StackPackBufferStringTest, ValidCStringTest1)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put(""), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(StackPackBufferStringTest, OverflowCStringTest)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  ASSERT_EQ(buffer->put("Hi not vs Hello but vs Hi"), false);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(StackPackBufferStringTest, InvalidCStringTest)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  ASSERT_EQ(buffer->put((char *) nullptr), false);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}