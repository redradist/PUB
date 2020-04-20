//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "pub/UnpackBuffer.hpp"

using buffers::UnpackBuffer;

struct UnpackBufferStringTest : testing::Test
{
  uint8_t array[100];
  UnpackBuffer * unpackBuffer;
  virtual void SetUp() {
    unpackBuffer = new UnpackBuffer(array);
  };

  virtual void TearDown() {
    delete unpackBuffer;
  };
};

TEST_F(UnpackBufferStringTest, ValidTest)
{
  const std::string testCase0 = "array0";
  std::copy(testCase0.c_str(), testCase0.c_str() + testCase0.size() + 1,
            array);
  const std::string testCase1 = "arra1";
  std::copy(testCase1.c_str(), testCase1.c_str() + testCase1.size() + 1,
            array + testCase0.size() + 1 + 1 /*alignment size*/);
  const std::string testCase2 = "arr2";
  std::copy(testCase2.c_str(), testCase2.c_str() + testCase2.size() + 1,
            array + testCase0.size() + 1 + 1 /*alignment size*/ + testCase1.size() + 1 + 2 /*alignment size*/);
  ASSERT_EQ(std::string(unpackBuffer->get()),
            std::string("array0"));
  ASSERT_EQ(std::string(unpackBuffer->get()),
            std::string("arra1"));
  ASSERT_EQ(std::string(unpackBuffer->get()),
            std::string("arr2"));
}

struct UnpackBufferByteTest : testing::Test
{
  uint8_t array[20];
  UnpackBuffer * unpackBuffer;
  virtual void SetUp() {
    unpackBuffer = new UnpackBuffer(array);
  };

  virtual void TearDown() {
    delete unpackBuffer;
  };
};

TEST_F(UnpackBufferByteTest, ValidTest)
{
  //
  array[0] = 5;
  array[1] = 0;
  array[2] = 0;
  array[3] = 0;
  //
  array[4] = 3;
  array[5] = 0;
  array[6] = 0;
  array[7] = 0;
  //
  array[8] = 6;
  array[9] = 0;
  array[10] = 0;
  array[11] = 0;
  ASSERT_EQ(unpackBuffer->get<uint8_t>(), 5);
  ASSERT_EQ(unpackBuffer->get<uint8_t>(), 3);
  ASSERT_EQ(unpackBuffer->get<uint8_t>(), 6);
}

struct UnpackBufferWordTest : testing::Test
{
  uint16_t array[20];
  UnpackBuffer * unpackBuffer;
  virtual void SetUp() {
    unpackBuffer = new UnpackBuffer(array);
  };

  virtual void TearDown() {
    delete unpackBuffer;
  };
};

TEST_F(UnpackBufferWordTest, ValidTest)
{
  //
  array[0] = 5;
  array[1] = 0;
  //
  array[2] = 3;
  array[3] = 0;
  //
  array[4] = 6;
  array[5] = 0;
  ASSERT_EQ(unpackBuffer->get<uint16_t>(), 5);
  ASSERT_EQ(unpackBuffer->get<uint16_t>(), 3);
  ASSERT_EQ(unpackBuffer->get<uint16_t>(), 6);
}

struct UnpackBufferDoubleWordTest : testing::Test
{
  uint32_t array[20];
  UnpackBuffer * unpackBuffer;
  virtual void SetUp() {
    unpackBuffer = new UnpackBuffer(array);
  };

  virtual void TearDown() {
    delete unpackBuffer;
  };
};

TEST_F(UnpackBufferDoubleWordTest, ValidTest)
{
  array[0] = 5;
  array[1] = 3;
  array[2] = 6;
  ASSERT_EQ(unpackBuffer->get<uint32_t>(), 5);
  ASSERT_EQ(unpackBuffer->get<uint32_t>(), 3);
  ASSERT_EQ(unpackBuffer->get<uint32_t>(), 6);
}