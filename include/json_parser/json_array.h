#ifndef JSON_PARSER_JSON_ARRAY_H_
#define JSON_PARSER_JSON_ARRAY_H_

#include "json_value.h"
#include "json_exception.h"

#include <string>
#include <vector>

namespace json_parser {

// JSON Array class - represents a JSON array
class JsonArray {
 public:
  using Iterator = std::vector<JsonValue>::iterator;
  using ConstIterator = std::vector<JsonValue>::const_iterator;

  JsonArray() = default;
  JsonArray(const JsonArray& other) = default;
  JsonArray(JsonArray&& other) noexcept = default;
  JsonArray& operator=(const JsonArray& other) = default;
  JsonArray& operator=(JsonArray&& other) noexcept = default;
  ~JsonArray() = default;

  // Element access
  JsonValue& operator[](size_t index);
  const JsonValue& operator[](size_t index) const;
  JsonValue& At(size_t index);
  const JsonValue& At(size_t index) const;
  JsonValue& Front();
  const JsonValue& Front() const;
  JsonValue& Back();
  const JsonValue& Back() const;

  // Capacity
  bool Empty() const { return values_.empty(); }
  size_t Size() const { return values_.size(); }
  void Reserve(size_t capacity) { values_.reserve(capacity); }

  // Modifiers
  void PushBack(const JsonValue& value);
  void PushBack(JsonValue&& value);
  void PopBack();
  void Insert(size_t index, const JsonValue& value);
  void Erase(size_t index);
  void Clear() { values_.clear(); }

  // Iterators
  Iterator Begin() { return values_.begin(); }
  ConstIterator Begin() const { return values_.cbegin(); }
  Iterator End() { return values_.end(); }
  ConstIterator End() const { return values_.cend(); }

  // String representation
  std::string ToString(int indent = 0) const;
  std::string ToCompactString() const;

  // Comparison
  bool operator==(const JsonArray& other) const;
  bool operator!=(const JsonArray& other) const;

 private:
  std::vector<JsonValue> values_;
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_ARRAY_H_

