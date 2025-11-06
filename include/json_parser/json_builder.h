#ifndef JSON_PARSER_JSON_BUILDER_H_
#define JSON_PARSER_JSON_BUILDER_H_

#include "json_value.h"
#include "json_exception.h"

#include <string>
#include <vector>

namespace json_parser {

// Builder pattern for constructing JSON values
class JsonBuilder {
 public:
  JsonBuilder();
  ~JsonBuilder() = default;

  // Build a JSON object
  JsonBuilder& StartObject();
  JsonBuilder& EndObject();
  JsonBuilder& Key(const std::string& key);
  JsonBuilder& Value(const JsonValue& value);
  JsonBuilder& Value(const std::string& value);
  JsonBuilder& Value(const char* value);
  JsonBuilder& Value(double value);
  JsonBuilder& Value(int value);
  JsonBuilder& Value(bool value);
  JsonBuilder& NullValue();

  // Build a JSON array
  JsonBuilder& StartArray();
  JsonBuilder& EndArray();
  JsonBuilder& AddValue(const JsonValue& value);
  JsonBuilder& AddValue(const std::string& value);
  JsonBuilder& AddValue(const char* value);
  JsonBuilder& AddValue(double value);
  JsonBuilder& AddValue(int value);
  JsonBuilder& AddValue(bool value);
  JsonBuilder& AddNull();

  // Get the built JSON value
  JsonValue Build();
  JsonValue GetResult() const { return result_; }

  // Reset builder
  void Reset();

 private:
  JsonValue result_;
  std::vector<JsonValue*> stack_;  // Stack for nested structures

  // Helper methods
  void PushValue(const JsonValue& value);
  JsonValue* GetCurrentContext();
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_BUILDER_H_

