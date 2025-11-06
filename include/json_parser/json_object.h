#ifndef JSON_PARSER_JSON_OBJECT_H_
#define JSON_PARSER_JSON_OBJECT_H_

#include "json_value.h"
#include "json_exception.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace json_parser {

// JSON Object class - represents a JSON object (key-value pairs)
class JsonObject {
 public:
  using Iterator = std::unordered_map<std::string, JsonValue>::iterator;
  using ConstIterator =
      std::unordered_map<std::string, JsonValue>::const_iterator;

  JsonObject() = default;
  JsonObject(const JsonObject& other) = default;
  JsonObject(JsonObject&& other) noexcept = default;
  JsonObject& operator=(const JsonObject& other) = default;
  JsonObject& operator=(JsonObject&& other) noexcept = default;
  ~JsonObject() = default;

  // Element access
  JsonValue& operator[](const std::string& key);
  const JsonValue& operator[](const std::string& key) const;
  JsonValue& At(const std::string& key);
  const JsonValue& At(const std::string& key) const;

  // Capacity
  bool Empty() const { return values_.empty(); }
  size_t Size() const { return values_.size(); }

  // Modifiers
  void Insert(const std::string& key, const JsonValue& value);
  void Insert(const std::string& key, JsonValue&& value);
  void Erase(const std::string& key);
  void Clear() { values_.clear(); }

  // Lookup
  bool Contains(const std::string& key) const;
  bool Has(const std::string& key) const { return Contains(key); }
  size_t Count(const std::string& key) const;

  // Iterators
  Iterator Begin() { return values_.begin(); }
  ConstIterator Begin() const { return values_.cbegin(); }
  Iterator End() { return values_.end(); }
  ConstIterator End() const { return values_.cend(); }

  // Get all keys
  std::vector<std::string> GetKeys() const;

  // String representation
  std::string ToString(int indent = 0) const;
  std::string ToCompactString() const;

  // Comparison
  bool operator==(const JsonObject& other) const;
  bool operator!=(const JsonObject& other) const;

 private:
  std::unordered_map<std::string, JsonValue> values_;
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_OBJECT_H_

