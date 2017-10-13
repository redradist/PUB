//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "Buffers/HeapPackBuffer.hpp"
#include "Buffers/UnpackBuffer.hpp"

using Buffers::HeapPackBuffer;
using Buffers::UnpackBuffer;

struct HeapPackBufferIntTest : testing::Test
{
  HeapPackBuffer * buffer;
  virtual void SetUp() {
    buffer = new HeapPackBuffer(3);
  };

  virtual void TearDown() {
    delete buffer;
  };
};

struct HeapPackBufferStringTest : testing::Test
{
  HeapPackBuffer * buffer;
  virtual void SetUp() {
    buffer = new HeapPackBuffer(30);
  };

  virtual void TearDown() {
    delete buffer;
  };
};

struct HeapPackBufferVectorTest : testing::Test
{
    HeapPackBuffer * buffer;
    virtual void SetUp() {
        buffer = new HeapPackBuffer(200);
    };

    virtual void TearDown() {
        delete buffer;
    };
};

struct HeapPackBufferMixedDataTest : testing::Test
{
  HeapPackBuffer * buffer;
  virtual void SetUp() {
    buffer = new HeapPackBuffer(120);
  };

  virtual void TearDown() {
    delete buffer;
  };
};

TEST_F(HeapPackBufferIntTest, ValidIntTest)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
}

TEST_F(HeapPackBufferIntTest, OverflowIntTest)
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

TEST_F(HeapPackBufferIntTest, CrashIntTest0)
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

TEST_F(HeapPackBufferIntTest, CrashIntTest1)
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

TEST_F(HeapPackBufferIntTest, CrashIntTest2)
{
  ASSERT_EQ(buffer->put(nullptr), false);
  ASSERT_EQ(buffer->put(nullptr), false);
}

TEST_F(HeapPackBufferStringTest, ValidStringTest0)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hello"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, ValidStringTest1)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, OverflowStringTest)
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

TEST_F(HeapPackBufferStringTest, ValidCStringTest0)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, ValidCStringTest1)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put(""), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, OverflowCStringTest)
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

TEST_F(HeapPackBufferStringTest, InvalidCStringTest)
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

TEST_F(HeapPackBufferVectorTest, ValidVectorgTest0)
{
  std::vector<int> vec0 = {1, 2, 3};
  std::vector<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), vec1);
}

TEST_F(HeapPackBufferVectorTest, ValidVectorgTest1)
{
  std::vector<double> vec0 = {1, 2, 3};
  std::vector<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::vector<double>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), vec1);
}

TEST_F(HeapPackBufferVectorTest, ValidListTest0)
{
  std::list<int> lst0 = {1, 2, 3};
  std::list<float> lst1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(lst0), true);
  ASSERT_EQ(buffer->put(lst1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::list<int>>(), lst0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), lst1);
}

TEST_F(HeapPackBufferVectorTest, ValidListTest1)
{
  std::list<double> lst0 = {1, 2, 3};
  std::list<float> lst1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(lst0), true);
  ASSERT_EQ(buffer->put(lst1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), lst1);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest0)
{
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put<double>(8.), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<double>(), 8.);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest1)
{
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest2)
{
  std::list<double> lst = {1, 2, 3};
  std::vector<int> vec = {1, 2, 3};
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put(lst), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put(vec), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
}
