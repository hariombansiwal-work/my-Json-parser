#include "json_parser/json_parser.h"
#include "json_parser/json_exception.h"
#include "json_parser/json_object.h"
#include "json_parser/json_array.h"

#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>

namespace json_parser {

// Constructor
JsonParser::JsonParser(const JsonParserConfig& config) : config_(config) {}

// Parse JSON from string (instance method)
JsonValue JsonParser::ParseString(const std::string& json) {
  Initialize(json);
  JsonValue result = ParseValue();
  SkipWhitespace();
  if (position_ < input_.length()) {
    ThrowParseError("Unexpected characters after JSON value");
  }
  return result;
}

JsonValue JsonParser::ParseString(const char* json, size_t length) {
  return ParseString(std::string(json, length));
}

// Parse JSON from file (instance method)
JsonValue JsonParser::ParseFileImpl(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw JsonFileException(filename);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return ParseString(buffer.str());
}

// Static convenience methods
JsonValue JsonParser::Parse(const std::string& json,
                            const JsonParserConfig& config) {
  JsonParser parser(config);
  return parser.ParseString(json);
}

JsonValue JsonParser::ParseFile(const std::string& filename,
                                 const JsonParserConfig& config) {
  JsonParser parser(config);
  return parser.ParseFileImpl(filename);
}

// Initialize parser state
void JsonParser::Initialize(const std::string& input) {
  input_ = input;
  position_ = 0;
  line_ = 1;
  column_ = 1;
}

// Parse methods
JsonValue JsonParser::ParseValue() {
  ValidateDepth(0);
  SkipWhitespace();
  if (config_.allow_comments) {
    SkipComments();
  }

  char c = Current();

  if (c == '{') {
    return JsonValue(ParseObject());
  } else if (c == '[') {
    return JsonValue(ParseArray());
  } else if (c == '"') {
    return JsonValue(ParseString());
  } else if (c == '-' || (c >= '0' && c <= '9')) {
    return JsonValue(ParseNumber());
  } else if (c == 't' || c == 'f') {
    return JsonValue(ParseBoolean());
  } else if (c == 'n') {
    ParseNull();
    return JsonValue(nullptr);
  } else {
    ThrowParseError("Unexpected character: " + std::string(1, c));
    return JsonValue(nullptr);  // Never reached
  }
}

JsonObject JsonParser::ParseObject() {
  ValidateDepth(0);
  JsonObject obj;
  Expect('{');
  SkipWhitespace();
  if (config_.allow_comments) {
    SkipComments();
  }

  if (Current() == '}') {
    Next();
    return obj;
  }

  while (true) {
    SkipWhitespace();
    if (config_.allow_comments) {
      SkipComments();
    }

    std::string key = ParseString();
    SkipWhitespace();
    if (config_.allow_comments) {
      SkipComments();
    }
    Expect(':');
    SkipWhitespace();
    if (config_.allow_comments) {
      SkipComments();
    }

    JsonValue value = ParseValue();
    obj.Insert(key, value);

    SkipWhitespace();
    if (config_.allow_comments) {
      SkipComments();
    }

    char c = Current();
    if (c == '}') {
      Next();
      break;
    } else if (c == ',') {
      Next();
    } else {
      ThrowParseError("Expected ',' or '}' in object");
    }
  }

  return obj;
}

JsonArray JsonParser::ParseArray() {
  ValidateDepth(0);
  JsonArray arr;
  Expect('[');
  SkipWhitespace();
  if (config_.allow_comments) {
    SkipComments();
  }

  if (Current() == ']') {
    Next();
    return arr;
  }

  while (true) {
    SkipWhitespace();
    if (config_.allow_comments) {
      SkipComments();
    }

    JsonValue value = ParseValue();

    SkipWhitespace();
    if (config_.allow_comments) {
      SkipComments();
    }

    char c = Current();
    if (c == ']') {
      if (config_.allow_trailing_commas && position_ > 0 &&
          input_[position_ - 1] == ',') {
        // Trailing comma allowed
      }
      Next();
      break;
    } else if (c == ',') {
      Next();
    } else {
      ThrowParseError("Expected ',' or ']' in array");
    }
  }

  return arr;
}

std::string JsonParser::ParseString() {
  Expect('"');
  std::string result;
  result.reserve(64);

  while (position_ < input_.length()) {
    char c = Next();
    if (c == '"') {
      break;
    } else if (c == '\\') {
      char esc = Next();
      switch (esc) {
        case '"':
          result += '"';
          break;
        case '\\':
          result += '\\';
          break;
        case '/':
          result += '/';
          break;
        case 'b':
          result += '\b';
          break;
        case 'f':
          result += '\f';
          break;
        case 'n':
          result += '\n';
          break;
        case 'r':
          result += '\r';
          break;
        case 't':
          result += '\t';
          break;
        case 'u': {
          // Unicode escape - simplified implementation
          for (int i = 0; i < 4; ++i) {
            if (position_ >= input_.length() ||
                !std::isxdigit(input_[position_])) {
              ThrowParseError("Invalid Unicode escape sequence");
            }
            Next();
          }
          // For now, we skip the actual Unicode character
          break;
        }
        default:
          result += esc;
          break;
      }
    } else if (c == '\n' || c == '\r') {
      if (config_.strict_mode) {
        ThrowParseError("Unescaped newline in string");
      }
      result += c;
    } else {
      result += c;
    }

    ValidateStringLength(result.length());
  }

  return result;
}

double JsonParser::ParseNumber() {
  bool negative = false;

  if (Current() == '-') {
    negative = true;
    Next();
  }

  double result = 0.0;
  while (position_ < input_.length() && std::isdigit(Current())) {
    result = result * 10 + (Current() - '0');
    Next();
  }

  if (Current() == '.') {
    Next();
    double fraction = 0.0;
    double divisor = 10.0;
    while (position_ < input_.length() && std::isdigit(Current())) {
      fraction += (Current() - '0') / divisor;
      divisor *= 10.0;
      Next();
    }
    result += fraction;
  }

  if (Current() == 'e' || Current() == 'E') {
    Next();
    bool exp_negative = false;
    if (Current() == '-') {
      exp_negative = true;
      Next();
    } else if (Current() == '+') {
      Next();
    }

    int exponent = 0;
    while (position_ < input_.length() && std::isdigit(Current())) {
      exponent = exponent * 10 + (Current() - '0');
      Next();
    }

    if (exp_negative) {
      exponent = -exponent;
    }
    result *= std::pow(10.0, exponent);
  }

  return negative ? -result : result;
}

bool JsonParser::ParseBoolean() {
  if (Match("true")) {
    return true;
  } else if (Match("false")) {
    return false;
  } else {
    ThrowParseError("Invalid boolean value");
    return false;  // Never reached
  }
}

void JsonParser::ParseNull() {
  if (!Match("null")) {
    ThrowParseError("Invalid null value");
  }
}

// Utility methods
void JsonParser::SkipWhitespace() {
  while (position_ < input_.length() && std::isspace(input_[position_])) {
    if (input_[position_] == '\n') {
      line_++;
      column_ = 1;
    } else {
      column_++;
    }
    position_++;
  }
}

void JsonParser::SkipComments() {
  if (position_ + 1 < input_.length()) {
    if (input_[position_] == '/' && input_[position_ + 1] == '/') {
      // Single-line comment
      while (position_ < input_.length() && input_[position_] != '\n') {
        position_++;
      }
      if (position_ < input_.length()) {
        position_++;  // Skip newline
        line_++;
        column_ = 1;
      }
    } else if (input_[position_] == '/' && input_[position_ + 1] == '*') {
      // Multi-line comment
      position_ += 2;
      while (position_ + 1 < input_.length()) {
        if (input_[position_] == '*' && input_[position_ + 1] == '/') {
          position_ += 2;
          break;
        }
        if (input_[position_] == '\n') {
          line_++;
          column_ = 1;
        } else {
          column_++;
        }
        position_++;
      }
    }
  }
}

char JsonParser::Current() const {
  if (position_ >= input_.length()) {
    ThrowParseError("Unexpected end of input");
  }
  return input_[position_];
}

char JsonParser::Next() {
  if (position_ >= input_.length()) {
    ThrowParseError("Unexpected end of input");
  }
  char c = input_[position_++];
  if (c == '\n') {
    line_++;
    column_ = 1;
  } else {
    column_++;
  }
  return c;
}

char JsonParser::Peek(size_t offset) const {
  size_t pos = position_ + offset;
  if (pos >= input_.length()) {
    return '\0';
  }
  return input_[pos];
}

bool JsonParser::Expect(char c) {
  if (Current() != c) {
    ThrowParseError("Expected '" + std::string(1, c) + "' but got '" +
                    std::string(1, Current()) + "'");
  }
  Next();
  return true;
}

bool JsonParser::Match(const std::string& str) {
  if (position_ + str.length() > input_.length()) {
    return false;
  }
  for (size_t i = 0; i < str.length(); ++i) {
    if (input_[position_ + i] != str[i]) {
      return false;
    }
  }
  Advance(str.length());
  return true;
}

void JsonParser::Advance(size_t count) {
  for (size_t i = 0; i < count; ++i) {
    if (position_ >= input_.length()) {
      break;
    }
    if (input_[position_] == '\n') {
      line_++;
      column_ = 1;
    } else {
      column_++;
    }
    position_++;
  }
}

// Error reporting
void JsonParser::ThrowParseError(const std::string& message) const {
  throw JsonParseException(message + " at line " + std::to_string(line_) +
                               ", column " + std::to_string(column_),
                           position_);
}

std::string JsonParser::GetContext() const {
  size_t start = (position_ > 20) ? position_ - 20 : 0;
  size_t end = (position_ + 20 < input_.length()) ? position_ + 20
                                                   : input_.length();
  return input_.substr(start, end - start);
}

// Validation
void JsonParser::ValidateDepth(size_t depth) const {
  if (depth > config_.max_depth) {
    throw JsonParseException("Maximum nesting depth exceeded");
  }
}

void JsonParser::ValidateStringLength(size_t length) const {
  if (length > config_.max_string_length) {
    throw JsonParseException("String length exceeds maximum allowed");
  }
}

}  // namespace json_parser

