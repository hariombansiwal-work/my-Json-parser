#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonParserTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(JsonParserTest, ParseSimpleObject) {
  std::string json = R"({"name": "John", "age": 30})";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.IsObject());
  EXPECT_EQ(value.AsObject()["name"].AsString(), "John");
  EXPECT_EQ(value.AsObject()["age"].AsNumber(), 30);
}

TEST_F(JsonParserTest, ParseSimpleArray) {
  std::string json = R"([1, 2, 3, "hello"])";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.IsArray());
  EXPECT_EQ(value.AsArray().Size(), 4);
  EXPECT_EQ(value.AsArray()[0].AsNumber(), 1);
  EXPECT_EQ(value.AsArray()[3].AsString(), "hello");
}

TEST_F(JsonParserTest, ParseNestedObject) {
  std::string json = R"({"user": {"name": "Alice", "id": 123}})";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.IsObject());
  JsonValue user = value.AsObject()["user"];
  EXPECT_TRUE(user.IsObject());
  EXPECT_EQ(user.AsObject()["name"].AsString(), "Alice");
  EXPECT_EQ(user.AsObject()["id"].AsNumber(), 123);
}

TEST_F(JsonParserTest, ParseNumbers) {
  std::string json = R"({"int": 42, "float": 3.14, "negative": -10})";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_DOUBLE_EQ(value.AsObject()["int"].AsNumber(), 42);
  EXPECT_DOUBLE_EQ(value.AsObject()["float"].AsNumber(), 3.14);
  EXPECT_DOUBLE_EQ(value.AsObject()["negative"].AsNumber(), -10);
}

TEST_F(JsonParserTest, ParseBooleans) {
  std::string json = R"({"true": true, "false": false})";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.AsObject()["true"].AsBoolean());
  EXPECT_FALSE(value.AsObject()["false"].AsBoolean());
}

TEST_F(JsonParserTest, ParseNull) {
  std::string json = R"({"value": null})";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.AsObject()["value"].IsNull());
}

TEST_F(JsonParserTest, ParseEmptyObject) {
  std::string json = "{}";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.IsObject());
  EXPECT_TRUE(value.AsObject().Empty());
}

TEST_F(JsonParserTest, ParseEmptyArray) {
  std::string json = "[]";
  JsonValue value = JsonParser::Parse(json);
  EXPECT_TRUE(value.IsArray());
  EXPECT_TRUE(value.AsArray().Empty());
}

TEST_F(JsonParserTest, ParseStringEscapes) {
  std::string json = R"({"text": "Hello\nWorld\tTest"})";
  JsonValue value = JsonParser::Parse(json);
  std::string text = value.AsObject()["text"].AsString();
  EXPECT_NE(text.find('\n'), std::string::npos);
  EXPECT_NE(text.find('\t'), std::string::npos);
}

TEST_F(JsonParserTest, ParseInvalidJson) {
  std::string json = "{invalid}";
  EXPECT_THROW(JsonParser::Parse(json), JsonParseException);
}

TEST_F(JsonParserTest, ParseUnclosedObject) {
  std::string json = R"({"key": "value")";
  EXPECT_THROW(JsonParser::Parse(json), JsonParseException);
}

TEST_F(JsonParserTest, ParseUnclosedArray) {
  std::string json = "[1, 2, 3";
  EXPECT_THROW(JsonParser::Parse(json), JsonParseException);
}

TEST_F(JsonParserTest, ParseWithConfig) {
  JsonParserConfig config = JsonParserConfig::Strict();
  std::string json = R"({"key": "value"})";
  JsonValue value = JsonParser::Parse(json, config);
  EXPECT_TRUE(value.IsObject());
}

