#ifndef JSON_PARSER_JSON_PARSER_H_
#define JSON_PARSER_JSON_PARSER_H_

#include "json_exception.h"
#include "json_value.h"

#include <string>

namespace json_parser {

// Parser configuration
struct JsonParserConfig {
  bool allow_comments = false;
  bool allow_trailing_commas = false;
  bool strict_mode = true;
  size_t max_depth = 1000;
  size_t max_string_length = 1000000;

  static JsonParserConfig Strict() {
    JsonParserConfig config;
    config.strict_mode = true;
    return config;
  }

  static JsonParserConfig Lenient() {
    JsonParserConfig config;
    config.strict_mode = false;
    config.allow_comments = true;
    config.allow_trailing_commas = true;
    return config;
  }
};

// Main JSON Parser class
class JsonParser {
 public:
  explicit JsonParser(const JsonParserConfig& config = JsonParserConfig::Strict());
  ~JsonParser() = default;

  // Parse JSON from string (instance method)
  JsonValue ParseString(const std::string& json);
  JsonValue ParseString(const char* json, size_t length);

  // Parse JSON from file (instance method)
  JsonValue ParseFileImpl(const std::string& filename);

  // Static convenience methods
  static JsonValue Parse(const std::string& json,
                         const JsonParserConfig& config = JsonParserConfig::Strict());
  static JsonValue ParseFile(const std::string& filename,
                             const JsonParserConfig& config = JsonParserConfig::Strict());

  // Get parser configuration
  const JsonParserConfig& GetConfig() const { return config_; }
  void SetConfig(const JsonParserConfig& config) { config_ = config; }

 private:
  JsonParserConfig config_;
  std::string input_;
  size_t position_;
  size_t line_;
  size_t column_;

  // Initialize parser state
  void Initialize(const std::string& input);

  // Parse methods
  JsonValue ParseValue();
  JsonObject ParseObject();
  JsonArray ParseArray();
  std::string ParseString();
  double ParseNumber();
  bool ParseBoolean();
  void ParseNull();

  // Utility methods
  void SkipWhitespace();
  void SkipComments();
  char Current() const;
  char Next();
  char Peek(size_t offset = 1) const;
  bool Expect(char c);
  bool Match(const std::string& str);
  void Advance(size_t count = 1);

  // Error reporting
  void ThrowParseError(const std::string& message) const;
  std::string GetContext() const;

  // Validation
  void ValidateDepth(size_t depth) const;
  void ValidateStringLength(size_t length) const;
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_PARSER_H_
