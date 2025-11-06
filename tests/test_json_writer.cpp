#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonWriterTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(JsonWriterTest, WriteSimpleObject) {
  JsonObject obj;
  obj.Insert("key", JsonValue("value"));
  JsonWriter writer;
  std::string result = writer.Write(obj);
  EXPECT_NE(result.find("key"), std::string::npos);
  EXPECT_NE(result.find("value"), std::string::npos);
}

TEST_F(JsonWriterTest, WriteCompact) {
  JsonObject obj;
  obj.Insert("key", JsonValue("value"));
  JsonWriter writer(JsonWriterConfig::Compact());
  std::string result = writer.Write(obj);
  EXPECT_EQ(result.find('\n'), std::string::npos);
}

TEST_F(JsonWriterTest, WritePretty) {
  JsonObject obj;
  obj.Insert("key", JsonValue("value"));
  JsonWriter writer(JsonWriterConfig::Pretty());
  std::string result = writer.Write(obj);
  EXPECT_NE(result.find('\n'), std::string::npos);
}

TEST_F(JsonWriterTest, WriteArray) {
  JsonArray arr;
  arr.PushBack(JsonValue(1));
  arr.PushBack(JsonValue(2));
  JsonWriter writer;
  std::string result = writer.Write(arr);
  EXPECT_NE(result.find("1"), std::string::npos);
  EXPECT_NE(result.find("2"), std::string::npos);
}

TEST_F(JsonWriterTest, WriteNested) {
  JsonObject outer;
  JsonObject inner;
  inner.Insert("nested", JsonValue("value"));
  outer.Insert("inner", JsonValue(inner));
  JsonWriter writer;
  std::string result = writer.Write(outer);
  EXPECT_NE(result.find("nested"), std::string::npos);
}

