#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonObjectTest : public ::testing::Test {
 protected:
  void SetUp() override {
    obj_.Insert("string", JsonValue("value"));
    obj_.Insert("number", JsonValue(42));
    obj_.Insert("boolean", JsonValue(true));
  }
  void TearDown() override {}

  JsonObject obj_;
};

TEST_F(JsonObjectTest, InsertAndAccess) {
  EXPECT_EQ(obj_["string"].AsString(), "value");
  EXPECT_EQ(obj_["number"].AsNumber(), 42);
  EXPECT_TRUE(obj_["boolean"].AsBoolean());
}

TEST_F(JsonObjectTest, Contains) {
  EXPECT_TRUE(obj_.Contains("string"));
  EXPECT_TRUE(obj_.Contains("number"));
  EXPECT_FALSE(obj_.Contains("nonexistent"));
}

TEST_F(JsonObjectTest, Size) {
  EXPECT_EQ(obj_.Size(), 3);
  obj_.Insert("new", JsonValue("key"));
  EXPECT_EQ(obj_.Size(), 4);
}

TEST_F(JsonObjectTest, Erase) {
  EXPECT_EQ(obj_.Size(), 3);
  obj_.Erase("string");
  EXPECT_EQ(obj_.Size(), 2);
  EXPECT_FALSE(obj_.Contains("string"));
}

TEST_F(JsonObjectTest, Clear) {
  EXPECT_FALSE(obj_.Empty());
  obj_.Clear();
  EXPECT_TRUE(obj_.Empty());
  EXPECT_EQ(obj_.Size(), 0);
}

TEST_F(JsonObjectTest, AtMethod) {
  EXPECT_EQ(obj_.At("string").AsString(), "value");
  EXPECT_THROW(obj_.At("nonexistent"), JsonKeyException);
}

TEST_F(JsonObjectTest, GetKeys) {
  auto keys = obj_.GetKeys();
  EXPECT_EQ(keys.size(), 3);
  EXPECT_NE(std::find(keys.begin(), keys.end(), "string"), keys.end());
}

TEST_F(JsonObjectTest, Equality) {
  JsonObject obj2;
  obj2.Insert("string", JsonValue("value"));
  obj2.Insert("number", JsonValue(42));
  obj2.Insert("boolean", JsonValue(true));
  EXPECT_EQ(obj_, obj2);
}

TEST_F(JsonObjectTest, ToString) {
  std::string str = obj_.ToString();
  EXPECT_NE(str.find("string"), std::string::npos);
  EXPECT_NE(str.find("value"), std::string::npos);
}

TEST_F(JsonObjectTest, EmptyObject) {
  JsonObject empty;
  EXPECT_TRUE(empty.Empty());
  EXPECT_EQ(empty.ToString(), "{}");
}

