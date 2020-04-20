//
// Created by redra on 29.04.17.
//

#include <gtest/gtest.h>
#include "pub/HeapPackBuffer.hpp"
#include "pub/UnpackBuffer.hpp"

using buffers::HeapPackBuffer;
using buffers::UnpackBuffer;

struct HeapPackBufferIntTest : testing::Test
{
  HeapPackBuffer * buffer;
  virtual void SetUp() {
    buffer = new HeapPackBuffer(12);
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
    buffer = new HeapPackBuffer(200);
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
}

TEST_F(HeapPackBufferIntTest, CrashIntTest0)
{
  ASSERT_EQ(buffer->put(uint8_t{ 1 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
//  ASSERT_EQ(buffer->put(nullptr), false); // Compilation error
  ASSERT_EQ(buffer->put(uint8_t{ 4 }), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 1);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 4);
}

TEST_F(HeapPackBufferIntTest, CrashIntTest1)
{
//  ASSERT_EQ(buffer->put(nullptr), false); // Compilation error
  ASSERT_EQ(buffer->put(uint8_t{ 2 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 3 }), true);
  ASSERT_EQ(buffer->put(uint8_t{ 4 }), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 2);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 3);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 4);
}

TEST_F(HeapPackBufferIntTest, CrashIntTest2)
{
//  ASSERT_EQ(buffer->put(nullptr), false); // Compilation error
//  ASSERT_EQ(buffer->put(nullptr), false); // Compilation error
}

TEST_F(HeapPackBufferStringTest, ValidStringTest0)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hello"}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, ValidStringTest1)
{
  ASSERT_EQ(buffer->put(std::string{"Hi"}), true);
  ASSERT_EQ(buffer->put(std::string{}), true);
  ASSERT_EQ(buffer->put(std::string{"Hi vs Hello"}), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, ValidCStringTest0)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get(), std::string{"Hi"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get(), std::string{"Hi vs Hello"});
}

TEST_F(HeapPackBufferStringTest, ValidCStringTest1)
{
  ASSERT_EQ(buffer->put("Hi"), true);
  ASSERT_EQ(buffer->put(""), true);
  ASSERT_EQ(buffer->put("Hi vs Hello"), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), vec1);
}

TEST_F(HeapPackBufferVectorTest, ValidVectorgTest1)
{
  std::vector<double> vec0 = {1, 2, 3};
  std::vector<float> vec1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(vec0), true);
  ASSERT_EQ(buffer->put(vec1), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<std::vector<double>>(), vec0);
  ASSERT_EQ(unbuffer.get<std::vector<float>>(), vec1);
}

TEST_F(HeapPackBufferVectorTest, ValidListTest0)
{
  std::list<int> lst0 = {1, 2, 3};
  std::list<float> lst1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(lst0), true);
  ASSERT_EQ(buffer->put(lst1), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<std::list<int>>(), lst0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), lst1);
}

TEST_F(HeapPackBufferVectorTest, ValidListTest1)
{
  std::list<double> lst0 = {1, 2, 3};
  std::list<float> lst1 = {3, 2, 1};
  ASSERT_EQ(buffer->put(lst0), true);
  ASSERT_EQ(buffer->put(lst1), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst0);
  ASSERT_EQ(unbuffer.get<std::list<float>>(), lst1);
}

TEST_F(HeapPackBufferVectorTest, ValidSetTest0)
{
  std::set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  ASSERT_EQ(buffer->put(set0), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::set<std::string>>();
  ASSERT_EQ(res0, set0);
}

TEST_F(HeapPackBufferVectorTest, ValidSetTest1)
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::set<std::string>>();
  ASSERT_EQ(res0, set0);
  auto res2 = unbuffer.get<std::set<std::string>>();
  ASSERT_EQ(res2, set1);
}

TEST_F(HeapPackBufferVectorTest, ValidPairTest0)
{
  std::pair<int, std::string> pair0;
  pair0.first = 8;
  pair0.second = "8";
  ASSERT_EQ(buffer->put(pair0), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto result = unbuffer.get<std::pair<int, std::string>>();
  ASSERT_EQ(result, pair0);
}

TEST_F(HeapPackBufferVectorTest, ValidPairTest1)
{
  std::pair<int, std::string> pair0;
  pair0.first = 8;
  pair0.second = "8";
  std::pair<std::string, int> pair1;
  pair1.first = "18";
  pair1.second = 18;
  ASSERT_EQ(buffer->put(pair0), true);
  ASSERT_EQ(buffer->put(pair1), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::pair<int, std::string>>();
  ASSERT_EQ(res0, pair0);
  auto res1 = unbuffer.get<std::pair<std::string, int>>();
  ASSERT_EQ(res1, pair1);
}

TEST_F(HeapPackBufferVectorTest, ValidMapTest0)
{
  std::map<int, std::string> map0;
  map0[8] = "4";
  map0[2] = "5";
  map0[3] = "8";
  ASSERT_EQ(buffer->put(map0), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto result = unbuffer.get<std::map<int, std::string>>();
  ASSERT_EQ(result, map0);
}

TEST_F(HeapPackBufferVectorTest, ValidMapTest1)
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::map<int, std::string>>();
  ASSERT_EQ(res0, map0);
  auto res1 = unbuffer.get<std::map<std::string, int>>();
  ASSERT_EQ(res1, map1);
}

TEST_F(HeapPackBufferVectorTest, ValidHashSetTest0)
{
  std::unordered_set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  ASSERT_EQ(buffer->put(set0), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::unordered_set<std::string>>();
  ASSERT_EQ(res0, set0);
}

TEST_F(HeapPackBufferVectorTest, ValidHashSetTest1)
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::unordered_set<std::string>>();
  ASSERT_EQ(res0, set0);
  auto res2 = unbuffer.get<std::unordered_set<std::string>>();
  ASSERT_EQ(res2, set1);
}

TEST_F(HeapPackBufferVectorTest, ValidHashMapTest0)
{
  std::unordered_map<int, std::string> map0;
  map0[8] = "4";
  map0[2] = "5";
  map0[3] = "8";
  ASSERT_EQ(buffer->put(map0), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto result = unbuffer.get<std::unordered_map<int, std::string>>();
  ASSERT_EQ(result, map0);
}

TEST_F(HeapPackBufferVectorTest, ValidHashMapTest1)
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  auto res0 = unbuffer.get<std::unordered_map<int, std::string>>();
  ASSERT_EQ(res0, map0);
  auto res1 = unbuffer.get<std::unordered_map<std::string, int>>();
  ASSERT_EQ(res1, map1);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest0)
{
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put<double>(8.), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<double>(), 8.);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest1)
{
  ASSERT_EQ(buffer->put("Hello"), true);
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  ASSERT_EQ(buffer->put<float>(8.), true);
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest3)
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
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
  ASSERT_EQ(unbuffer.get(), std::string{"Hello"});
  ASSERT_EQ(unbuffer.get<std::list<double>>(), lst);
  ASSERT_EQ(unbuffer.get<uint8_t>(), 8);
  ASSERT_EQ(unbuffer.get<std::vector<int>>(), vec);
  ASSERT_EQ(unbuffer.get<std::set<std::string>>(), set);
  ASSERT_EQ(unbuffer.get<float>(), 8.);
  auto res0 = unbuffer.get<std::map<std::string, int>>();
  ASSERT_EQ(res0, map);
}

TEST_F(HeapPackBufferMixedDataTest, MixedDataTest4)
{
  std::list<double> lst = {1, 2, 3};
  ASSERT_EQ(buffer->getTypeSize("Hello"), 6);
  std::unordered_set<std::string> set0;
  set0.insert("4");
  set0.insert("5");
  set0.insert("8");
  ASSERT_EQ(buffer->getTypeSize(set0), 14);
  std::vector<int> vec = {1, 2, 3};
  ASSERT_EQ(buffer->getTypeSize(vec), 20);
  std::unordered_map<std::string, int> map0;
  map0["1"] = 1;
  map0["8"] = 6;
  map0["5"] = 9;
  ASSERT_EQ(buffer->getTypeSize(map0), 26);
  std::set<std::string> set1;
  set1.insert("4");
  set1.insert("5");
  set1.insert("8");
  ASSERT_EQ(buffer->getTypeSize(set1), 14);
  std::map<std::string, int> map1;
  map1["1"] = 1;
  map1["8"] = 6;
  map1["5"] = 9;
  ASSERT_EQ(buffer->getTypeSize(map1), 26);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put("Hello"), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put(set0), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put(lst), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put<uint8_t>(8), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put(vec), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put(map0), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put(set1), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  ASSERT_EQ(buffer->put<float>(8.), true);
  ASSERT_EQ(buffer->put(map1), true);
  std::cout << "Buffer size: " << buffer->getBufferSize() << std::endl;
  UnpackBuffer unbuffer(buffer->getData(), buffer->getDataSize());
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
