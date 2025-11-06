#include "json_parser/json_value.h"
#include "json_parser/json_exception.h"
#include "json_parser/json_object.h"
#include "json_parser/json_array.h"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace json_parser {

// Constructors
JsonValue::JsonValue() : type_(JsonValueType::kNull), value_(nullptr) {}

JsonValue::JsonValue(const JsonObject& obj)
    : type_(JsonValueType::kObject),
      value_(std::make_shared<JsonObject>(obj)) {}

JsonValue::JsonValue(const JsonArray& arr)
    : type_(JsonValueType::kArray),
      value_(std::make_shared<JsonArray>(arr)) {}

JsonValue::JsonValue(const std::string& str)
    : type_(JsonValueType::kString), value_(str) {}

JsonValue::JsonValue(const char* str)
    : type_(JsonValueType::kString), value_(std::string(str)) {}

JsonValue::JsonValue(double num)
    : type_(JsonValueType::kNumber), value_(num) {}

JsonValue::JsonValue(int num)
    : type_(JsonValueType::kNumber), value_(static_cast<double>(num)) {}

JsonValue::JsonValue(bool b)
    : type_(JsonValueType::kBoolean), value_(b) {}

JsonValue::JsonValue(std::nullptr_t)
    : type_(JsonValueType::kNull), value_(nullptr) {}

// Copy and move constructors
JsonValue::JsonValue(const JsonValue& other)
    : type_(other.type_), value_(other.value_) {}

JsonValue::JsonValue(JsonValue&& other) noexcept
    : type_(other.type_), value_(std::move(other.value_)) {
  other.type_ = JsonValueType::kNull;
  other.value_ = nullptr;
}

JsonValue& JsonValue::operator=(const JsonValue& other) {
  if (this != &other) {
    type_ = other.type_;
    value_ = other.value_;
  }
  return *this;
}

JsonValue& JsonValue::operator=(JsonValue&& other) noexcept {
  if (this != &other) {
    type_ = other.type_;
    value_ = std::move(other.value_);
    other.type_ = JsonValueType::kNull;
    other.value_ = nullptr;
  }
  return *this;
}

// Type-safe accessors
JsonObject& JsonValue::AsObject() {
  ValidateType(JsonValueType::kObject);
  return *std::get<std::shared_ptr<JsonObject>>(value_);
}

const JsonObject& JsonValue::AsObject() const {
  ValidateType(JsonValueType::kObject);
  return *std::get<std::shared_ptr<JsonObject>>(value_);
}

JsonArray& JsonValue::AsArray() {
  ValidateType(JsonValueType::kArray);
  return *std::get<std::shared_ptr<JsonArray>>(value_);
}

const JsonArray& JsonValue::AsArray() const {
  ValidateType(JsonValueType::kArray);
  return *std::get<std::shared_ptr<JsonArray>>(value_);
}

std::string& JsonValue::AsString() {
  ValidateType(JsonValueType::kString);
  return std::get<std::string>(value_);
}

const std::string& JsonValue::AsString() const {
  ValidateType(JsonValueType::kString);
  return std::get<std::string>(value_);
}

double& JsonValue::AsNumber() {
  ValidateType(JsonValueType::kNumber);
  return std::get<double>(value_);
}

const double& JsonValue::AsNumber() const {
  ValidateType(JsonValueType::kNumber);
  return std::get<double>(value_);
}

bool& JsonValue::AsBoolean() {
  ValidateType(JsonValueType::kBoolean);
  return std::get<bool>(value_);
}

const bool& JsonValue::AsBoolean() const {
  ValidateType(JsonValueType::kBoolean);
  return std::get<bool>(value_);
}

// Safe accessors with default values
const JsonObject& JsonValue::AsObjectOrDefault(
    const JsonObject& default_val) const {
  if (IsObject()) {
    return AsObject();
  }
  return default_val;
}

const JsonArray& JsonValue::AsArrayOrDefault(
    const JsonArray& default_val) const {
  if (IsArray()) {
    return AsArray();
  }
  return default_val;
}

const std::string& JsonValue::AsStringOrDefault(
    const std::string& default_val) const {
  if (IsString()) {
    return AsString();
  }
  return default_val;
}

double JsonValue::AsNumberOrDefault(double default_val) const {
  if (IsNumber()) {
    return AsNumber();
  }
  return default_val;
}

bool JsonValue::AsBooleanOrDefault(bool default_val) const {
  if (IsBoolean()) {
    return AsBoolean();
  }
  return default_val;
}

// Comparison operators
bool JsonValue::operator==(const JsonValue& other) const {
  if (type_ != other.type_) {
    return false;
  }

  switch (type_) {
    case JsonValueType::kObject:
      return *std::get<std::shared_ptr<JsonObject>>(value_) ==
             *std::get<std::shared_ptr<JsonObject>>(other.value_);
    case JsonValueType::kArray:
      return *std::get<std::shared_ptr<JsonArray>>(value_) ==
             *std::get<std::shared_ptr<JsonArray>>(other.value_);
    case JsonValueType::kString:
      return std::get<std::string>(value_) ==
             std::get<std::string>(other.value_);
    case JsonValueType::kNumber:
      return std::abs(std::get<double>(value_) -
                      std::get<double>(other.value_)) < 1e-9;
    case JsonValueType::kBoolean:
      return std::get<bool>(value_) == std::get<bool>(other.value_);
    case JsonValueType::kNull:
      return true;
  }
  return false;
}

bool JsonValue::operator!=(const JsonValue& other) const {
  return !(*this == other);
}

// String representation
std::string JsonValue::ToString(int indent) const {
  switch (type_) {
    case JsonValueType::kObject:
      return std::get<std::shared_ptr<JsonObject>>(value_)->ToString(indent);
    case JsonValueType::kArray:
      return std::get<std::shared_ptr<JsonArray>>(value_)->ToString(indent);
    case JsonValueType::kString: {
      std::ostringstream oss;
      oss << "\"";
      const std::string& str = std::get<std::string>(value_);
      for (char c : str) {
        if (c == '"') {
          oss << "\\\"";
        } else if (c == '\\') {
          oss << "\\\\";
        } else if (c == '\n') {
          oss << "\\n";
        } else if (c == '\r') {
          oss << "\\r";
        } else if (c == '\t') {
          oss << "\\t";
        } else {
          oss << c;
        }
      }
      oss << "\"";
      return oss.str();
    }
    case JsonValueType::kNumber: {
      std::ostringstream oss;
      double num = std::get<double>(value_);
      if (num == static_cast<long long>(num)) {
        oss << static_cast<long long>(num);
      } else {
        oss << std::fixed << std::setprecision(6) << num;
      }
      return oss.str();
    }
    case JsonValueType::kBoolean:
      return std::get<bool>(value_) ? "true" : "false";
    case JsonValueType::kNull:
      return "null";
  }
  return "null";
}

std::string JsonValue::ToCompactString() const {
  return ToString(0);
}

// Helper methods
void JsonValue::ValidateType(JsonValueType expected) const {
  if (type_ != expected) {
    std::string type_name;
    switch (type_) {
      case JsonValueType::kObject:
        type_name = "object";
        break;
      case JsonValueType::kArray:
        type_name = "array";
        break;
      case JsonValueType::kString:
        type_name = "string";
        break;
      case JsonValueType::kNumber:
        type_name = "number";
        break;
      case JsonValueType::kBoolean:
        type_name = "boolean";
        break;
      case JsonValueType::kNull:
        type_name = "null";
        break;
    }
    throw JsonTypeException("Expected different type, got: " + type_name);
  }
}

}  // namespace json_parser

