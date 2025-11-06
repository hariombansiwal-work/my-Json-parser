#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonValueTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(JsonValueTest, DefaultConstructor) {
  JsonValue value;
  EXPECT_TRUE(value.IsNull());
  EXPECT_EQ(value.GetType(), JsonValueType::kNull);
}

TEST_F(JsonValueTest, StringConstructor) {
  JsonValue value("hello");
  EXPECT_TRUE(value.IsString());
  EXPECT_EQ(value.AsString(), "hello");
}

TEST_F(JsonValueTest, NumberConstructor) {
  JsonValue value(42.5);
  EXPECT_TRUE(value.IsNumber());
  EXPECT_DOUBLE_EQ(value.AsNumber(), 42.5);
}

TEST_F(JsonValueTest, BooleanConstructor) {
  JsonValue value(true);
  EXPECT_TRUE(value.IsBoolean());
  EXPECT_TRUE(value.AsBoolean());
}

TEST_F(JsonValueTest, ObjectConstructor) {
  JsonObject obj;
  obj.Insert("key", JsonValue("value"));
  JsonValue value(obj);
  EXPECT_TRUE(value.IsObject());
  EXPECT_EQ(value.AsObject().Size(), 1);
}

TEST_F(JsonValueTest, ArrayConstructor) {
  JsonArray arr;
  arr.PushBack(JsonValue(1));
  JsonValue value(arr);
  EXPECT_TRUE(value.IsArray());
  EXPECT_EQ(value.AsArray().Size(), 1);
}

TEST_F(JsonValueTest, CopyConstructor) {
  JsonValue original("test");
  JsonValue copy(original);
  EXPECT_EQ(original.AsString(), copy.AsString());
  EXPECT_TRUE(copy.IsString());
}

TEST_F(JsonValueTest, MoveConstructor) {
  JsonValue original("test");
  JsonValue moved(std::move(original));
  EXPECT_EQ(moved.AsString(), "test");
  EXPECT_TRUE(original.IsNull());
}

TEST_F(JsonValueTest, EqualityOperator) {
  JsonValue v1("hello");
  JsonValue v2("hello");
  JsonValue v3("world");
  EXPECT_EQ(v1, v2);
  EXPECT_NE(v1, v3);
}

TEST_F(JsonValueTest, TypeChecking) {
  JsonValue str("test");
  JsonValue num(42);
  JsonValue boolean(true);
  JsonValue null_val(nullptr);

  EXPECT_TRUE(str.IsString());
  EXPECT_FALSE(str.IsNumber());
  EXPECT_TRUE(num.IsNumber());
  EXPECT_TRUE(boolean.IsBoolean());
  EXPECT_TRUE(null_val.IsNull());
}

TEST_F(JsonValueTest, SafeAccessors) {
  JsonValue value("test");
  EXPECT_EQ(value.AsStringOrDefault("default"), "test");
  EXPECT_EQ(value.AsNumberOrDefault(0.0), 0.0);
  EXPECT_EQ(value.AsStringOrDefault("default"), "test");
}

TEST_F(JsonValueTest, ToString) {
  JsonValue str("hello");
  JsonValue num(42);
  JsonValue boolean(true);
  JsonValue null_val(nullptr);

  EXPECT_NE(str.ToString().find("hello"), std::string::npos);
  EXPECT_NE(num.ToString().find("42"), std::string::npos);
  EXPECT_EQ(boolean.ToString(), "true");
  EXPECT_EQ(null_val.ToString(), "null");
}

TEST_F(JsonValueTest, TypeException) {
  JsonValue value("test");
  EXPECT_THROW(value.AsNumber(), JsonTypeException);
  EXPECT_THROW(value.AsBoolean(), JsonTypeException);
  EXPECT_THROW(value.AsObject(), JsonTypeException);
  EXPECT_THROW(value.AsArray(), JsonTypeException);
}

