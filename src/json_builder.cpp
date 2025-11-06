#include "json_parser/json_builder.h"
#include "json_parser/json_exception.h"
#include "json_parser/json_object.h"
#include "json_parser/json_array.h"
#include "json_parser/json_value.h"

#include <stdexcept>
#include <vector>

namespace json_parser {

JsonBuilder::JsonBuilder() : result_(nullptr) {}

JsonBuilder& JsonBuilder::StartObject() {
  JsonObject obj_obj;
  JsonValue obj(obj_obj);
  if (stack_.empty()) {
    result_ = obj;
    stack_.push_back(&result_);
  } else {
    JsonValue* context = GetCurrentContext();
    if (context->IsArray()) {
      context->AsArray().PushBack(obj);
      JsonArray& arr = context->AsArray();
      stack_.push_back(&arr[arr.Size() - 1]);
    } else if (context->IsObject()) {
      // Should be set via Key() first
      throw JsonException("StartObject called without Key in object context");
    } else {
      *context = obj;
    }
  }
  return *this;
}

JsonBuilder& JsonBuilder::EndObject() {
  if (stack_.empty() || !stack_.back()->IsObject()) {
    throw JsonException("EndObject called without matching StartObject");
  }
  stack_.pop_back();
  return *this;
}

JsonBuilder& JsonBuilder::Key(const std::string& key) {
  if (stack_.empty() || !stack_.back()->IsObject()) {
    throw JsonException("Key called outside of object context");
  }
  JsonObject& obj = stack_.back()->AsObject();
  obj.Insert(key, JsonValue(nullptr));
  // Store reference to the inserted value for later setting
  return *this;
}

JsonBuilder& JsonBuilder::Value(const JsonValue& value) {
  PushValue(value);
  return *this;
}

JsonBuilder& JsonBuilder::Value(const std::string& value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::Value(const char* value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::Value(double value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::Value(int value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::Value(bool value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::NullValue() {
  PushValue(JsonValue(nullptr));
  return *this;
}

JsonBuilder& JsonBuilder::StartArray() {
  JsonArray arr_obj;
  JsonValue arr(arr_obj);
  if (stack_.empty()) {
    result_ = arr;
    stack_.push_back(&result_);
  } else {
    JsonValue* context = GetCurrentContext();
    if (context->IsArray()) {
      context->AsArray().PushBack(arr);
      JsonArray& parent_arr = context->AsArray();
      stack_.push_back(&parent_arr[parent_arr.Size() - 1]);
    } else if (context->IsObject()) {
      // Should be set via Key() first
      throw JsonException("StartArray called without Key in object context");
    } else {
      *context = arr;
    }
  }
  return *this;
}

JsonBuilder& JsonBuilder::EndArray() {
  if (stack_.empty() || !stack_.back()->IsArray()) {
    throw JsonException("EndArray called without matching StartArray");
  }
  stack_.pop_back();
  return *this;
}

JsonBuilder& JsonBuilder::AddValue(const JsonValue& value) {
  PushValue(value);
  return *this;
}

JsonBuilder& JsonBuilder::AddValue(const std::string& value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::AddValue(const char* value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::AddValue(double value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::AddValue(int value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::AddValue(bool value) {
  PushValue(JsonValue(value));
  return *this;
}

JsonBuilder& JsonBuilder::AddNull() {
  PushValue(JsonValue(nullptr));
  return *this;
}

JsonValue JsonBuilder::Build() {
  if (!stack_.empty()) {
    throw JsonException("Cannot build: unclosed structures remain");
  }
  return result_;
}

void JsonBuilder::Reset() {
  result_ = JsonValue(nullptr);
  stack_.clear();
}

void JsonBuilder::PushValue(const JsonValue& value) {
  if (stack_.empty()) {
    result_ = value;
    stack_.push_back(&result_);
  } else {
    JsonValue* context = GetCurrentContext();
    if (context->IsArray()) {
      context->AsArray().PushBack(value);
    } else {
      // Replace the current value
      *context = value;
    }
  }
}

JsonValue* JsonBuilder::GetCurrentContext() {
  if (stack_.empty()) {
    return &result_;
  }
  return stack_.back();
}

}  // namespace json_parser

