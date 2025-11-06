#ifndef JSON_PARSER_JSON_VALUE_H_
#define JSON_PARSER_JSON_VALUE_H_

#include <memory>
#include <string>
#include <variant>

namespace json_parser {

// Forward declarations
class JsonObject;
class JsonArray;

// JSON value types
enum class JsonValueType {
  kObject,
  kArray,
  kString,
  kNumber,
  kBoolean,
  kNull
};

// Main JSON value container class
class JsonValue {
 public:
  // Constructors
  JsonValue();
  explicit JsonValue(const JsonObject& obj);
  explicit JsonValue(const JsonArray& arr);
  explicit JsonValue(const std::string& str);
  explicit JsonValue(const char* str);
  explicit JsonValue(double num);
  explicit JsonValue(int num);
  explicit JsonValue(bool b);
  explicit JsonValue(std::nullptr_t);

  // Copy and move constructors
  JsonValue(const JsonValue& other);
  JsonValue(JsonValue&& other) noexcept;
  JsonValue& operator=(const JsonValue& other);
  JsonValue& operator=(JsonValue&& other) noexcept;

  // Destructor
  ~JsonValue() = default;

  // Type checking
  JsonValueType GetType() const { return type_; }
  bool IsObject() const { return type_ == JsonValueType::kObject; }
  bool IsArray() const { return type_ == JsonValueType::kArray; }
  bool IsString() const { return type_ == JsonValueType::kString; }
  bool IsNumber() const { return type_ == JsonValueType::kNumber; }
  bool IsBoolean() const { return type_ == JsonValueType::kBoolean; }
  bool IsNull() const { return type_ == JsonValueType::kNull; }

  // Type-safe accessors
  JsonObject& AsObject();
  const JsonObject& AsObject() const;
  JsonArray& AsArray();
  const JsonArray& AsArray() const;
  std::string& AsString();
  const std::string& AsString() const;
  double& AsNumber();
  const double& AsNumber() const;
  bool& AsBoolean();
  const bool& AsBoolean() const;

  // Safe accessors with default values
  const JsonObject& AsObjectOrDefault(const JsonObject& default_val) const;
  const JsonArray& AsArrayOrDefault(const JsonArray& default_val) const;
  const std::string& AsStringOrDefault(
      const std::string& default_val) const;
  double AsNumberOrDefault(double default_val) const;
  bool AsBooleanOrDefault(bool default_val) const;

  // Comparison operators
  bool operator==(const JsonValue& other) const;
  bool operator!=(const JsonValue& other) const;

  // String representation
  std::string ToString(int indent = 0) const;
  std::string ToCompactString() const;

 private:
  JsonValueType type_;
  std::variant<std::shared_ptr<JsonObject>, std::shared_ptr<JsonArray>,
               std::string, double, bool, std::nullptr_t>
      value_;

  // Helper methods for type checking in accessors
  void ValidateType(JsonValueType expected) const;
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_VALUE_H_

