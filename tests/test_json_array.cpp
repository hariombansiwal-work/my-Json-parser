#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonArrayTest : public ::testing::Test {
 protected:
  void SetUp() override {
    arr_.PushBack(JsonValue(1));
    arr_.PushBack(JsonValue("two"));
    arr_.PushBack(JsonValue(true));
  }
  void TearDown() override {}

  JsonArray arr_;
};

TEST_F(JsonArrayTest, PushBackAndAccess) {
  EXPECT_EQ(arr_.Size(), 3);
  EXPECT_EQ(arr_[0].AsNumber(), 1);
  EXPECT_EQ(arr_[1].AsString(), "two");
  EXPECT_TRUE(arr_[2].AsBoolean());
}

TEST_F(JsonArrayTest, Size) {
  EXPECT_EQ(arr_.Size(), 3);
  arr_.PushBack(JsonValue(4));
  EXPECT_EQ(arr_.Size(), 4);
}

TEST_F(JsonArrayTest, Empty) {
  JsonArray empty;
  EXPECT_TRUE(empty.Empty());
  EXPECT_FALSE(arr_.Empty());
}

TEST_F(JsonArrayTest, Clear) {
  EXPECT_FALSE(arr_.Empty());
  arr_.Clear();
  EXPECT_TRUE(arr_.Empty());
}

TEST_F(JsonArrayTest, AtMethod) {
  EXPECT_EQ(arr_.At(0).AsNumber(), 1);
  EXPECT_THROW(arr_.At(10), JsonException);
}

TEST_F(JsonArrayTest, FrontAndBack) {
  EXPECT_EQ(arr_.Front().AsNumber(), 1);
  EXPECT_TRUE(arr_.Back().AsBoolean());
}

TEST_F(JsonArrayTest, PopBack) {
  size_t initial_size = arr_.Size();
  arr_.PopBack();
  EXPECT_EQ(arr_.Size(), initial_size - 1);
}

TEST_F(JsonArrayTest, Insert) {
  arr_.Insert(1, JsonValue("inserted"));
  EXPECT_EQ(arr_.Size(), 4);
  EXPECT_EQ(arr_[1].AsString(), "inserted");
}

TEST_F(JsonArrayTest, Erase) {
  size_t initial_size = arr_.Size();
  arr_.Erase(1);
  EXPECT_EQ(arr_.Size(), initial_size - 1);
  EXPECT_EQ(arr_[1].AsBoolean(), true);
}

TEST_F(JsonArrayTest, Equality) {
  JsonArray arr2;
  arr2.PushBack(JsonValue(1));
  arr2.PushBack(JsonValue("two"));
  arr2.PushBack(JsonValue(true));
  EXPECT_EQ(arr_, arr2);
}

TEST_F(JsonArrayTest, ToString) {
  std::string str = arr_.ToString();
  EXPECT_NE(str.find("1"), std::string::npos);
  EXPECT_NE(str.find("two"), std::string::npos);
}

TEST_F(JsonArrayTest, EmptyArray) {
  JsonArray empty;
  EXPECT_TRUE(empty.Empty());
  EXPECT_EQ(empty.ToString(), "[]");
}

