#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class JsonBuilderTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(JsonBuilderTest, BuildSimpleObject) {
  JsonBuilder builder;
  builder.StartObject()
      .Key("name")
      .Value("John")
      .Key("age")
      .Value(30)
      .EndObject();
  JsonValue result = builder.Build();
  EXPECT_TRUE(result.IsObject());
  EXPECT_EQ(result.AsObject()["name"].AsString(), "John");
}

TEST_F(JsonBuilderTest, BuildArray) {
  JsonBuilder builder;
  builder.StartArray()
      .AddValue(1)
      .AddValue(2)
      .AddValue(3)
      .EndArray();
  JsonValue result = builder.Build();
  EXPECT_TRUE(result.IsArray());
  EXPECT_EQ(result.AsArray().Size(), 3);
}

TEST_F(JsonBuilderTest, BuildNested) {
  JsonBuilder builder;
  builder.StartObject()
      .Key("items")
      .StartArray()
      .AddValue(1)
      .AddValue(2)
      .EndArray()
      .EndObject();
  JsonValue result = builder.Build();
  EXPECT_TRUE(result.IsObject());
  EXPECT_TRUE(result.AsObject()["items"].IsArray());
}

