#include "json_parser/json_array.h"
#include "json_parser/json_exception.h"

namespace json_parser {

// Element access
JsonValue& JsonArray::operator[](size_t index) {
  if (index >= values_.size()) {
    throw JsonException("Array index out of bounds: " +
                        std::to_string(index));
  }
  return values_[index];
}

const JsonValue& JsonArray::operator[](size_t index) const {
  if (index >= values_.size()) {
    throw JsonException("Array index out of bounds: " +
                        std::to_string(index));
  }
  return values_[index];
}

JsonValue& JsonArray::At(size_t index) {
  if (index >= values_.size()) {
    throw JsonException("Array index out of bounds: " +
                        std::to_string(index));
  }
  return values_[index];
}

const JsonValue& JsonArray::At(size_t index) const {
  if (index >= values_.size()) {
    throw JsonException("Array index out of bounds: " +
                        std::to_string(index));
  }
  return values_[index];
}

JsonValue& JsonArray::Front() {
  if (values_.empty()) {
    throw JsonException("Array is empty");
  }
  return values_.front();
}

const JsonValue& JsonArray::Front() const {
  if (values_.empty()) {
    throw JsonException("Array is empty");
  }
  return values_.front();
}

JsonValue& JsonArray::Back() {
  if (values_.empty()) {
    throw JsonException("Array is empty");
  }
  return values_.back();
}

const JsonValue& JsonArray::Back() const {
  if (values_.empty()) {
    throw JsonException("Array is empty");
  }
  return values_.back();
}

// Modifiers
void JsonArray::PushBack(const JsonValue& value) {
  values_.push_back(value);
}

void JsonArray::PushBack(JsonValue&& value) {
  values_.push_back(std::move(value));
}

void JsonArray::PopBack() {
  if (values_.empty()) {
    throw JsonException("Cannot pop from empty array");
  }
  values_.pop_back();
}

void JsonArray::Insert(size_t index, const JsonValue& value) {
  if (index > values_.size()) {
    throw JsonException("Array index out of bounds: " +
                        std::to_string(index));
  }
  values_.insert(values_.begin() + index, value);
}

void JsonArray::Erase(size_t index) {
  if (index >= values_.size()) {
    throw JsonException("Array index out of bounds: " +
                        std::to_string(index));
  }
  values_.erase(values_.begin() + index);
}

// String representation
std::string JsonArray::ToString(int indent) const {
  if (values_.empty()) {
    return "[]";
  }

  std::string result = "[\n";
  std::string indent_str(indent + 2, ' ');

  for (size_t i = 0; i < values_.size(); ++i) {
    if (i > 0) {
      result += ",\n";
    }
    result += indent_str + values_[i].ToString(indent + 2);
  }

  result += "\n" + std::string(indent, ' ') + "]";
  return result;
}

std::string JsonArray::ToCompactString() const {
  if (values_.empty()) {
    return "[]";
  }

  std::string result = "[";
  for (size_t i = 0; i < values_.size(); ++i) {
    if (i > 0) {
      result += ",";
    }
    result += values_[i].ToCompactString();
  }
  result += "]";
  return result;
}

// Comparison
bool JsonArray::operator==(const JsonArray& other) const {
  if (values_.size() != other.values_.size()) {
    return false;
  }

  for (size_t i = 0; i < values_.size(); ++i) {
    if (values_[i] != other.values_[i]) {
      return false;
    }
  }

  return true;
}

bool JsonArray::operator!=(const JsonArray& other) const {
  return !(*this == other);
}

}  // namespace json_parser

