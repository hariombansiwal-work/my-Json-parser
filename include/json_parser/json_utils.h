#ifndef JSON_PARSER_JSON_UTILS_H_
#define JSON_PARSER_JSON_UTILS_H_

#include "json_value.h"

#include <string>

namespace json_parser {

// Utility functions for working with JSON
class JsonUtils {
 public:
  // Validation
  static bool IsValidJson(const std::string& json);
  static bool IsValidJsonFile(const std::string& filename);

  // Type conversion helpers
  static int AsInt(const JsonValue& value, int default_val = 0);
  static long AsLong(const JsonValue& value, long default_val = 0);
  static double AsDouble(const JsonValue& value, double default_val = 0.0);
  static std::string AsString(const JsonValue& value,
                              const std::string& default_val = "");
  static bool AsBool(const JsonValue& value, bool default_val = false);

  // Path-based access (e.g., "user.name" or "items[0]")
  static JsonValue* GetByPath(JsonValue& root, const std::string& path);
  static const JsonValue* GetByPath(const JsonValue& root,
                                     const std::string& path);
  static bool HasPath(const JsonValue& root, const std::string& path);

  // Deep copy
  static JsonValue DeepCopy(const JsonValue& value);

  // Merge two JSON objects
  static JsonObject Merge(const JsonObject& obj1, const JsonObject& obj2,
                          bool overwrite = true);

  // Pretty print with custom settings
  static std::string PrettyPrint(const JsonValue& value, int indent = 2);
  static std::string CompactPrint(const JsonValue& value);

  // Minify JSON string
  static std::string Minify(const std::string& json);

  // Format JSON string (pretty print)
  static std::string Format(const std::string& json, int indent = 2);

 private:
  JsonUtils() = default;  // Utility class, no instantiation
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_UTILS_H_

