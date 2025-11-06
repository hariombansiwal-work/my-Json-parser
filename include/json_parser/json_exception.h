#ifndef JSON_PARSER_JSON_EXCEPTION_H_
#define JSON_PARSER_JSON_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace json_parser {

// Base exception class for all JSON parser errors
class JsonException : public std::runtime_error {
 public:
  explicit JsonException(const std::string& message)
      : std::runtime_error(message) {}
  explicit JsonException(const char* message)
      : std::runtime_error(message) {}
};

// Exception thrown when parsing fails
class JsonParseException : public JsonException {
 public:
  explicit JsonParseException(const std::string& message)
      : JsonException("JSON Parse Error: " + message) {}
  explicit JsonParseException(const char* message)
      : JsonException(std::string("JSON Parse Error: ") + message) {}
  JsonParseException(const std::string& message, size_t position)
      : JsonException("JSON Parse Error at position " +
                      std::to_string(position) + ": " + message),
        position_(position) {}

  size_t GetPosition() const { return position_; }

 private:
  size_t position_ = 0;
};

// Exception thrown when accessing wrong type
class JsonTypeException : public JsonException {
 public:
  explicit JsonTypeException(const std::string& message)
      : JsonException("JSON Type Error: " + message) {}
  explicit JsonTypeException(const char* message)
      : JsonException(std::string("JSON Type Error: ") + message) {}
};

// Exception thrown when key is not found
class JsonKeyException : public JsonException {
 public:
  explicit JsonKeyException(const std::string& key)
      : JsonException("JSON Key Error: Key '" + key + "' not found") {}
};

// Exception thrown for file I/O errors
class JsonFileException : public JsonException {
 public:
  explicit JsonFileException(const std::string& filename)
      : JsonException("JSON File Error: Cannot open file '" + filename + "'") {}
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_EXCEPTION_H_

