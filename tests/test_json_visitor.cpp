#include <gtest/gtest.h>
#include "json_parser.h"

using namespace json_parser;

class CountingVisitor : public JsonVisitor {
 public:
  int object_count = 0;
  int array_count = 0;
  int string_count = 0;
  int number_count = 0;
  int boolean_count = 0;
  int null_count = 0;

  void VisitValue(const JsonValue& value) override {
    switch (value.GetType()) {
      case JsonValueType::kObject:
        VisitObject(value.AsObject());
        break;
      case JsonValueType::kArray:
        VisitArray(value.AsArray());
        break;
      case JsonValueType::kString:
        VisitString(value.AsString());
        break;
      case JsonValueType::kNumber:
        VisitNumber(value.AsNumber());
        break;
      case JsonValueType::kBoolean:
        VisitBoolean(value.AsBoolean());
        break;
      case JsonValueType::kNull:
        VisitNull();
        break;
    }
  }

  void VisitObject(const JsonObject& obj) override {
    object_count++;
    for (auto it = obj.Begin(); it != obj.End(); ++it) {
      VisitValue(it->second);
    }
  }

  void VisitArray(const JsonArray& arr) override {
    array_count++;
    for (size_t i = 0; i < arr.Size(); ++i) {
      VisitValue(arr[i]);
    }
  }

  void VisitString(const std::string& str) override { string_count++; }
  void VisitNumber(double num) override { number_count++; }
  void VisitBoolean(bool b) override { boolean_count++; }
  void VisitNull() override { null_count++; }
};

class JsonVisitorTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(JsonVisitorTest, VisitObject) {
  JsonObject obj;
  obj.Insert("key1", JsonValue("value1"));
  obj.Insert("key2", JsonValue(42));
  CountingVisitor visitor;
  visitor.VisitObject(obj);
  EXPECT_EQ(visitor.object_count, 1);
  EXPECT_EQ(visitor.string_count, 1);
  EXPECT_EQ(visitor.number_count, 1);
}

TEST_F(JsonVisitorTest, VisitArray) {
  JsonArray arr;
  arr.PushBack(JsonValue(1));
  arr.PushBack(JsonValue(2));
  CountingVisitor visitor;
  visitor.VisitArray(arr);
  EXPECT_EQ(visitor.array_count, 1);
  EXPECT_EQ(visitor.number_count, 2);
}

