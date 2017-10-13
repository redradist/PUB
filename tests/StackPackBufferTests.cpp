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

struct StackPackBufferVectorTest : testing::Test
{
  StackPackBuffer<120> * buffer;
  virtual void SetUp() {
    buffer = new StackPackBuffer<120>();
  };

  virtual void TearDown() {
    delete buffer;
  };
};

struct StackPackBufferMixedDataTest : testing::Test
{
  StackPackBuffer<120> * buffer;
  virtual void SetUp() {
    buffer = new StackPackBuffer<120>();
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

TEST_F(StackPackBufferVectorTest, ValidVectorgTest0)
{
  std::vector<int> vec0 = {1, 2, 3};
  std::vector<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res_vec0 = std::vector<int>{1, 2, 3};
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), res_vec0);
  auto res_vec1 = std::vector<float>{3, 2, 1};
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), res_vec1);
}

TEST_F(StackPackBufferVectorTest, ValidVectorgTest1)
{
  std::vector<double> vec0 = {1, 2, 3};
  std::vector<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res_vec0 = std::vector<double>{1, 2, 3};
  ASSERT_EQ(unbuffer.get<std::vector<double>>(), res_vec0);
  auto res_vec1 = std::vector<float>{3, 2, 1};
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), res_vec1);
}

TEST_F(StackPackBufferVectorTest, ValidListTest0)
{
  std::list<int> vec0 = {1, 2, 3};
  std::list<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res_vec0 = std::list<int>{1, 2, 3};
  ASSERT_EQ(unbuffer.get<std::list<int>>(), res_vec0);
  auto res_vec1 = std::list<float>{3, 2, 1};
  ASSERT_EQ(unbuffer.get<std::list<float>>(), res_vec1);
}

TEST_F(StackPackBufferVectorTest, ValidListTest1)
{
  std::list<double> vec0 = {1, 2, 3};
  std::list<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::list<double>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), vec1);
}

TEST_F(StackPackBufferMixedDataTest, MixedDataTest0)
{
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put<double>(8.), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<double>(), 8.);
}

TEST_F(StackPackBufferMixedDataTest, MixedDataTest1)
{
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
}

TEST_F(StackPackBufferMixedDataTest, MixedDataTest2)
{
  std::list<double> vec0 = {1, 2, 3};
  std::vector<int> vec1 = {1, 2, 3};
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put(vec1), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<std::list<double>>(), vec0);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec1);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
}