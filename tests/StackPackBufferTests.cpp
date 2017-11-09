//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "src/StackPackBuffer.hpp"
#include "src/UnpackBuffer.hpp"

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
  StackPackBuffer<200> * buffer;
  virtual void SetUp() {
    buffer = new StackPackBuffer<200>();
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
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), vec1);
}

TEST_F(StackPackBufferVectorTest, ValidVectorgTest1)
{
  std::vector<double> vec0 = {1, 2, 3};
  std::vector<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::vector<double>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), vec1);
}

TEST_F(StackPackBufferVectorTest, ValidListTest0)
{
  std::list<int> lst0 = {1, 2, 3};
  std::list<float> lst1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(lst0), true);
  ASSERT_EQ(buffer->put(lst1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::list<int>>(), lst0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), lst1);
}

TEST_F(StackPackBufferVectorTest, ValidListTest1)
{
  std::list<double> lst0 = {1, 2, 3};
  std::list<float> lst1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(lst0), true);
  ASSERT_EQ(buffer->put(lst1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), lst1);
}

TEST_F(StackPackBufferVectorTest, ValidSetTest0)
{
  std::set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  ASSERT_EQ(buffer->put(set0), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::set<std::string>>();
  ASSERT_EQ(res0, set0);
}

TEST_F(StackPackBufferVectorTest, ValidSetTest1)
{
  std::set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  std::set<std::string> set1;
  set1.insert("8");
  set1.insert("11");
  set1.insert("16");
  ASSERT_EQ(buffer->put(set0), true);
  ASSERT_EQ(buffer->put(set1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::set<std::string>>();
  ASSERT_EQ(res0, set0);
  auto res2 = unbuffer.get<std::set<std::string>>();
  ASSERT_EQ(res2, set1);
}

TEST_F(StackPackBufferVectorTest, ValidPairTest0)
{
  std::pair<int, std::string> pair0;
  pair0.first = 8;
  pair0.second = "8";
  ASSERT_EQ(buffer->put(pair0), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto result = unbuffer.get<std::pair<int, std::string>>();
  ASSERT_EQ(result, pair0);
}

TEST_F(StackPackBufferVectorTest, ValidPairTest1)
{
  std::pair<int, std::string> pair0;
  pair0.first = 8;
  pair0.second = "8";
  std::pair<std::string, int> pair1;
  pair1.first = "18";
  pair1.second = 18;
  ASSERT_EQ(buffer->put(pair0), true);
  ASSERT_EQ(buffer->put(pair1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::pair<int, std::string>>();
  ASSERT_EQ(res0, pair0);
  auto res1 = unbuffer.get<std::pair<std::string, int>>();
  ASSERT_EQ(res1, pair1);
}

TEST_F(StackPackBufferVectorTest, ValidMapTest0)
{
  std::map<int, std::string> map0;
  map0[8] = "4";
  map0[2] = "5";
  map0[3] = "8";
  ASSERT_EQ(buffer->put(map0), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto result = unbuffer.get<std::map<int, std::string>>();
  ASSERT_EQ(result, map0);
}

TEST_F(StackPackBufferVectorTest, ValidMapTest1)
{
  std::map<int, std::string> map0;
  map0[8] = "4";
  map0[2] = "5";
  map0[3] = "8";
  std::map<std::string, int> map1;
  map1["1"] = 1;
  map1["8"] = 6;
  map1["5"] = 9;
  ASSERT_EQ(buffer->put(map0), true);
  ASSERT_EQ(buffer->put(map1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::map<int, std::string>>();
  ASSERT_EQ(res0, map0);
  auto res1 = unbuffer.get<std::map<std::string, int>>();
  ASSERT_EQ(res1, map1);
}

TEST_F(StackPackBufferVectorTest, ValidHashSetTest0)
{
  std::unordered_set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  ASSERT_EQ(buffer->put(set0), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::unordered_set<std::string>>();
  ASSERT_EQ(res0, set0);
}

TEST_F(StackPackBufferVectorTest, ValidHashSetTest1)
{
  std::unordered_set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  std::unordered_set<std::string> set1;
  set1.insert("8");
  set1.insert("11");
  set1.insert("16");
  ASSERT_EQ(buffer->put(set0), true);
  ASSERT_EQ(buffer->put(set1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::unordered_set<std::string>>();
  ASSERT_EQ(res0, set0);
  auto res2 = unbuffer.get<std::unordered_set<std::string>>();
  ASSERT_EQ(res2, set1);
}

TEST_F(StackPackBufferVectorTest, ValidHashMapTest0)
{
  std::unordered_map<int, std::string> map0;
  map0[8] = "4";
  map0[2] = "5";
  map0[3] = "8";
  ASSERT_EQ(buffer->put(map0), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto result = unbuffer.get<std::unordered_map<int, std::string>>();
  ASSERT_EQ(result, map0);
}

TEST_F(StackPackBufferVectorTest, ValidHashMapTest1)
{
  std::unordered_map<int, std::string> map0;
  map0[8] = "4";
  map0[2] = "5";
  map0[3] = "8";
  std::unordered_map<std::string, int> map1;
  map1["1"] = 1;
  map1["8"] = 6;
  map1["5"] = 9;
  ASSERT_EQ(buffer->put(map0), true);
  ASSERT_EQ(buffer->put(map1), true);
  UnpackBuffer unbuffer(buffer->getData());
  auto res0 = unbuffer.get<std::unordered_map<int, std::string>>();
  ASSERT_EQ(res0, map0);
  auto res1 = unbuffer.get<std::unordered_map<std::string, int>>();
  ASSERT_EQ(res1, map1);
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

TEST_F(StackPackBufferMixedDataTest, MixedDataTest3)
{
  std::list<double> lst = {1, 2, 3};
  std::vector<int> vec = {1, 2, 3};
  std::set<std::string> set;
  set.insert("4");
  set.insert("5");
  set.insert("8");
  std::map<std::string, int> map;
  map["1"] = 1;
  map["8"] = 6;
  map["5"] = 9;
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put(lst), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put(vec), true);
  ASSERT_EQ(buffer->put(set), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  ASSERT_EQ(buffer->put(map), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec);
  ASSERT_EQ(unbuffer.get<std::set<std::string>>(), set);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
  auto res0 = unbuffer.get<std::map<std::string, int>>();
  ASSERT_EQ(res0, map);
}

TEST_F(StackPackBufferMixedDataTest, MixedDataTest4)
{
  std::list<double> lst = {1, 2, 3};
  std::unordered_set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  std::vector<int> vec = {1, 2, 3};
  std::unordered_map<std::string, int> map0;
  map0["1"] = 1;
  map0["8"] = 6;
  map0["5"] = 9;
  std::set<std::string> set1;
  set1.insert("4");
  set1.insert("5");
  set1.insert("8");
  std::map<std::string, int> map1;
  map1["1"] = 1;
  map1["8"] = 6;
  map1["5"] = 9;
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put(set0), true);
  ASSERT_EQ(buffer->put(lst), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put(vec), true);
  ASSERT_EQ(buffer->put(map0), true);
  ASSERT_EQ(buffer->put(set1), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  ASSERT_EQ(buffer->put(map1), true);
  UnpackBuffer unbuffer(buffer->getData());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<std::unordered_set<std::string>>(), set0);
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec);
  auto res0 = unbuffer.get<std::unordered_map<std::string, int>>();
  ASSERT_EQ(res0, map0);
  ASSERT_EQ(unbuffer.get<std::set<std::string>>(), set1);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
  auto res1 = unbuffer.get<std::map<std::string, int>>();
  ASSERT_EQ(res1, map1);
}
