#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonUtilsTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(JsonUtilsTest, IsValidJson) {
  EXPECT_TRUE(JsonUtils::IsValidJson(R"({"key": "value"})"));
  EXPECT_FALSE(JsonUtils::IsValidJson("{invalid}"));
}

TEST_F(JsonUtilsTest, TypeConversions) {
  JsonValue num(42);
  JsonValue str("hello");
  JsonValue boolean(true);

  EXPECT_EQ(JsonUtils::AsInt(num), 42);
  EXPECT_EQ(JsonUtils::AsString(str), "hello");
  EXPECT_TRUE(JsonUtils::AsBool(boolean));
}

TEST_F(JsonUtilsTest, GetByPath) {
  std::string json = R"({"user": {"name": "Alice", "id": 123}})";
  JsonValue value = JsonParser::Parse(json);
  const JsonValue* result = JsonUtils::GetByPath(value, "user.name");
  EXPECT_NE(result, nullptr);
  EXPECT_EQ(result->AsString(), "Alice");
}

TEST_F(JsonUtilsTest, HasPath) {
  std::string json = R"({"user": {"name": "Alice"}})";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(JsonUtils::HasPath(value, "user.name"));
  EXPECT_FALSE(JsonUtils::HasPath(value, "user.age"));
}

TEST_F(JsonUtilsTest, DeepCopy) {
  JsonObject obj;
  obj.Insert("key", JsonValue("value"));
  JsonValue original(obj);
  JsonValue copy = JsonUtils::DeepCopy(original);
  EXPECT_EQ(original, copy);
}

TEST_F(JsonUtilsTest, PrettyPrint) {
  std::string json = R"({"key":"value"})";
  JsonValue value = JsonParser::Parse(json);
  std::string formatted = JsonUtils::PrettyPrint(value);
  EXPECT_NE(formatted.find('\n'), std::string::npos);
}

TEST_F(JsonUtilsTest, CompactPrint) {
  JsonObject obj;
  obj.Insert("key", JsonValue("value"));
  JsonValue value(obj);
  std::string compact = JsonUtils::CompactPrint(value);
  EXPECT_EQ(compact.find('\n'), std::string::npos);
}

