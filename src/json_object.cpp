#include "json_parser/json_object.h"
#include "json_parser/json_exception.h"

#include <algorithm>

namespace json_parser {

// Element access
JsonValue& JsonObject::operator[](const std::string& key) {
  return values_[key];
}

const JsonValue& JsonObject::operator[](const std::string& key) const {
  auto it = values_.find(key);
  if (it == values_.end()) {
    throw JsonKeyException(key);
  }
  return it->second;
}

JsonValue& JsonObject::At(const std::string& key) {
  auto it = values_.find(key);
  if (it == values_.end()) {
    throw JsonKeyException(key);
  }
  return it->second;
}

const JsonValue& JsonObject::At(const std::string& key) const {
  auto it = values_.find(key);
  if (it == values_.end()) {
    throw JsonKeyException(key);
  }
  return it->second;
}

// Modifiers
void JsonObject::Insert(const std::string& key, const JsonValue& value) {
  values_[key] = value;
}

void JsonObject::Insert(const std::string& key, JsonValue&& value) {
  values_[key] = std::move(value);
}

void JsonObject::Erase(const std::string& key) {
  values_.erase(key);
}

// Lookup
bool JsonObject::Contains(const std::string& key) const {
  return values_.find(key) != values_.end();
}

size_t JsonObject::Count(const std::string& key) const {
  return values_.count(key);
}

// Get all keys
std::vector<std::string> JsonObject::GetKeys() const {
  std::vector<std::string> keys;
  keys.reserve(values_.size());
  for (const auto& pair : values_) {
    keys.push_back(pair.first);
  }
  return keys;
}

// String representation
std::string JsonObject::ToString(int indent) const {
  if (values_.empty()) {
    return "{}";
  }

  std::string result = "{\n";
  std::string indent_str(indent + 2, ' ');
  bool first = true;

  for (const auto& pair : values_) {
    if (!first) {
      result += ",\n";
    }
    first = false;
    result += indent_str + "\"" + pair.first + "\": " +
              pair.second.ToString(indent + 2);
  }

  result += "\n" + std::string(indent, ' ') + "}";
  return result;
}

std::string JsonObject::ToCompactString() const {
  if (values_.empty()) {
    return "{}";
  }

  std::string result = "{";
  bool first = true;

  for (const auto& pair : values_) {
    if (!first) {
      result += ",";
    }
    first = false;
    result += "\"" + pair.first + "\":" + pair.second.ToCompactString();
  }

  result += "}";
  return result;
}

// Comparison
bool JsonObject::operator==(const JsonObject& other) const {
  if (values_.size() != other.values_.size()) {
    return false;
  }

  for (const auto& pair : values_) {
    auto it = other.values_.find(pair.first);
    if (it == other.values_.end() || it->second != pair.second) {
      return false;
    }
  }

  return true;
}

bool JsonObject::operator!=(const JsonObject& other) const {
  return !(*this == other);
}

}  // namespace json_parser

