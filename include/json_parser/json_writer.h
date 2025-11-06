#ifndef JSON_PARSER_JSON_WRITER_H_
#define JSON_PARSER_JSON_WRITER_H_

#include "json_value.h"
#include "json_exception.h"

#include <iosfwd>
#include <string>

namespace json_parser {

// Configuration for JSON writing/formatting
struct JsonWriterConfig {
  bool pretty_print = true;
  int indent_size = 2;
  bool escape_unicode = false;
  bool sort_keys = false;
  int max_depth = 1000;

  static JsonWriterConfig Compact() {
    JsonWriterConfig config;
    config.pretty_print = false;
    return config;
  }

  static JsonWriterConfig Pretty() {
    JsonWriterConfig config;
    config.pretty_print = true;
    config.indent_size = 2;
    return config;
  }
};

// JSON Writer class - handles serialization of JSON to string
class JsonWriter {
 public:
  explicit JsonWriter(const JsonWriterConfig& config = JsonWriterConfig::Pretty());
  ~JsonWriter() = default;

  // Write JSON value to string
  std::string Write(const JsonValue& value) const;
  std::string Write(const JsonObject& obj) const;
  std::string Write(const JsonArray& arr) const;

  // Write to stream
  void WriteToStream(std::ostream& os, const JsonValue& value) const;
  void WriteToStream(std::ostream& os, const JsonObject& obj) const;
  void WriteToStream(std::ostream& os, const JsonArray& arr) const;

  // Write to file
  void WriteToFile(const std::string& filename, const JsonValue& value) const;

  // Update configuration
  void SetConfig(const JsonWriterConfig& config) { config_ = config; }
  const JsonWriterConfig& GetConfig() const { return config_; }

 private:
  JsonWriterConfig config_;

  // Helper methods
  void WriteValue(std::ostream& os, const JsonValue& value, int indent,
                  int depth) const;
  void WriteObject(std::ostream& os, const JsonObject& obj, int indent,
                   int depth) const;
  void WriteArray(std::ostream& os, const JsonArray& arr, int indent,
                  int depth) const;
  void WriteString(std::ostream& os, const std::string& str) const;
  void WriteNumber(std::ostream& os, double num) const;
  void WriteIndent(std::ostream& os, int indent) const;
  std::string EscapeString(const std::string& str) const;
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_WRITER_H_

