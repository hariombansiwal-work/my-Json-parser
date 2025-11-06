#include "json_parser/json_writer.h"
#include "json_parser/json_object.h"
#include "json_parser/json_array.h"
#include "json_parser/json_exception.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace json_parser {

JsonWriter::JsonWriter(const JsonWriterConfig& config) : config_(config) {}

std::string JsonWriter::Write(const JsonValue& value) const {
  std::ostringstream oss;
  WriteToStream(oss, value);
  return oss.str();
}

std::string JsonWriter::Write(const JsonObject& obj) const {
  std::ostringstream oss;
  WriteToStream(oss, obj);
  return oss.str();
}

std::string JsonWriter::Write(const JsonArray& arr) const {
  std::ostringstream oss;
  WriteToStream(oss, arr);
  return oss.str();
}

void JsonWriter::WriteToStream(std::ostream& os, const JsonValue& value) const {
  WriteValue(os, value, 0, 0);
}

void JsonWriter::WriteToStream(std::ostream& os,
                                const JsonObject& obj) const {
  WriteObject(os, obj, 0, 0);
}

void JsonWriter::WriteToStream(std::ostream& os, const JsonArray& arr) const {
  WriteArray(os, arr, 0, 0);
}

void JsonWriter::WriteToFile(const std::string& filename,
                              const JsonValue& value) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw JsonFileException(filename);
  }
  WriteToStream(file, value);
}

void JsonWriter::WriteValue(std::ostream& os, const JsonValue& value,
                             int indent, int depth) const {
  if (depth > config_.max_depth) {
    throw JsonException("Maximum depth exceeded during writing");
  }

  switch (value.GetType()) {
    case JsonValueType::kObject:
      WriteObject(os, value.AsObject(), indent, depth + 1);
      break;
    case JsonValueType::kArray:
      WriteArray(os, value.AsArray(), indent, depth + 1);
      break;
    case JsonValueType::kString:
      WriteString(os, value.AsString());
      break;
    case JsonValueType::kNumber:
      WriteNumber(os, value.AsNumber());
      break;
    case JsonValueType::kBoolean:
      os << (value.AsBoolean() ? "true" : "false");
      break;
    case JsonValueType::kNull:
      os << "null";
      break;
  }
}

void JsonWriter::WriteObject(std::ostream& os, const JsonObject& obj,
                              int indent, int depth) const {
  if (obj.Empty()) {
    os << "{}";
    return;
  }

  os << "{";
  if (config_.pretty_print) {
    os << "\n";
  }

  std::vector<std::string> keys = obj.GetKeys();
  if (config_.sort_keys) {
    std::sort(keys.begin(), keys.end());
  }

  bool first = true;
  for (const std::string& key : keys) {
    if (!first) {
      os << ",";
      if (config_.pretty_print) {
        os << "\n";
      }
    }
    first = false;

    if (config_.pretty_print) {
      WriteIndent(os, indent + config_.indent_size);
    }

    WriteString(os, key);
    os << (config_.pretty_print ? ": " : ":");

    const JsonValue& value = obj.At(key);
    WriteValue(os, value, indent + config_.indent_size, depth);
  }

  if (config_.pretty_print) {
    os << "\n";
    WriteIndent(os, indent);
  }
  os << "}";
}

void JsonWriter::WriteArray(std::ostream& os, const JsonArray& arr,
                             int indent, int depth) const {
  if (arr.Empty()) {
    os << "[]";
    return;
  }

  os << "[";
  if (config_.pretty_print) {
    os << "\n";
  }

  for (size_t i = 0; i < arr.Size(); ++i) {
    if (i > 0) {
      os << ",";
      if (config_.pretty_print) {
        os << "\n";
      }
    }

    if (config_.pretty_print) {
      WriteIndent(os, indent + config_.indent_size);
    }

    WriteValue(os, arr[i], indent + config_.indent_size, depth);
  }

  if (config_.pretty_print) {
    os << "\n";
    WriteIndent(os, indent);
  }
  os << "]";
}

void JsonWriter::WriteString(std::ostream& os, const std::string& str) const {
  os << "\"";
  os << EscapeString(str);
  os << "\"";
}

void JsonWriter::WriteNumber(std::ostream& os, double num) const {
  if (std::isnan(num) || std::isinf(num)) {
    os << "null";
    return;
  }

  if (num == static_cast<long long>(num)) {
    os << static_cast<long long>(num);
  } else {
    os << std::fixed << std::setprecision(6) << num;
  }
}

void JsonWriter::WriteIndent(std::ostream& os, int indent) const {
  for (int i = 0; i < indent; ++i) {
    os << " ";
  }
}

std::string JsonWriter::EscapeString(const std::string& str) const {
  std::string result;
  result.reserve(str.length() + 10);

  for (char c : str) {
    switch (c) {
      case '"':
        result += "\\\"";
        break;
      case '\\':
        result += "\\\\";
        break;
      case '\b':
        result += "\\b";
        break;
      case '\f':
        result += "\\f";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        if (config_.escape_unicode && (static_cast<unsigned char>(c) > 127)) {
          // Unicode escape
          std::ostringstream oss;
          oss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
              << static_cast<int>(static_cast<unsigned char>(c));
          result += oss.str();
        } else {
          result += c;
        }
        break;
    }
  }

  return result;
}

}  // namespace json_parser

