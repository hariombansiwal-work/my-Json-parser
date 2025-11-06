#include "json_parser/json_utils.h"
#include "json_parser/json_parser.h"
#include "json_parser/json_writer.h"
#include "json_parser/json_exception.h"
#include "json_parser/json_object.h"
#include "json_parser/json_array.h"

#include <algorithm>
#include <sstream>

namespace json_parser {

bool JsonUtils::IsValidJson(const std::string& json) {
  try {
    JsonParser::Parse(json);
    return true;
  } catch (const JsonException&) {
    return false;
  }
}

bool JsonUtils::IsValidJsonFile(const std::string& filename) {
  try {
    JsonParser::ParseFile(filename);
    return true;
  } catch (const JsonException&) {
    return false;
  }
}

int JsonUtils::AsInt(const JsonValue& value, int default_val) {
  if (value.IsNumber()) {
    return static_cast<int>(value.AsNumber());
  }
  return default_val;
}

long JsonUtils::AsLong(const JsonValue& value, long default_val) {
  if (value.IsNumber()) {
    return static_cast<long>(value.AsNumber());
  }
  return default_val;
}

double JsonUtils::AsDouble(const JsonValue& value, double default_val) {
  if (value.IsNumber()) {
    return value.AsNumber();
  }
  return default_val;
}

std::string JsonUtils::AsString(const JsonValue& value,
                                 const std::string& default_val) {
  if (value.IsString()) {
    return value.AsString();
  }
  return default_val;
}

bool JsonUtils::AsBool(const JsonValue& value, bool default_val) {
  if (value.IsBoolean()) {
    return value.AsBoolean();
  }
  return default_val;
}

JsonValue* JsonUtils::GetByPath(JsonValue& root, const std::string& path) {
  const JsonValue* const_result = GetByPath(static_cast<const JsonValue&>(root),
                                            path);
  return const_cast<JsonValue*>(const_result);
}

const JsonValue* JsonUtils::GetByPath(const JsonValue& root,
                                       const std::string& path) {
  const JsonValue* current = &root;
  std::istringstream iss(path);
  std::string segment;

  while (std::getline(iss, segment, '.')) {
    size_t bracket_pos = segment.find('[');
    if (bracket_pos != std::string::npos) {
      // Array access
      std::string key = segment.substr(0, bracket_pos);
      if (!key.empty()) {
        if (!current->IsObject()) {
          return nullptr;
        }
        current = &current->AsObject().At(key);
      }

      std::string index_str = segment.substr(bracket_pos + 1);
      index_str.pop_back();  // Remove ']'
      size_t index = std::stoul(index_str);

      if (!current->IsArray() || index >= current->AsArray().Size()) {
        return nullptr;
      }
      current = &current->AsArray()[index];
    } else {
      // Object key access
      if (!current->IsObject()) {
        return nullptr;
      }
      if (!current->AsObject().Contains(segment)) {
        return nullptr;
      }
      current = &current->AsObject().At(segment);
    }
  }

  return current;
}

bool JsonUtils::HasPath(const JsonValue& root, const std::string& path) {
  return GetByPath(root, path) != nullptr;
}

JsonValue JsonUtils::DeepCopy(const JsonValue& value) {
  switch (value.GetType()) {
    case JsonValueType::kObject: {
      JsonObject new_obj;
      const JsonObject& obj = value.AsObject();
      for (auto it = obj.Begin(); it != obj.End(); ++it) {
        new_obj.Insert(it->first, DeepCopy(it->second));
      }
      return JsonValue(new_obj);
    }
    case JsonValueType::kArray: {
      JsonArray new_arr;
      for (size_t i = 0; i < value.AsArray().Size(); ++i) {
        new_arr.PushBack(DeepCopy(value.AsArray()[i]));
      }
      return JsonValue(new_arr);
    }
    case JsonValueType::kString:
      return JsonValue(value.AsString());
    case JsonValueType::kNumber:
      return JsonValue(value.AsNumber());
    case JsonValueType::kBoolean:
      return JsonValue(value.AsBoolean());
    case JsonValueType::kNull:
      return JsonValue(nullptr);
  }
  return JsonValue(nullptr);
}

JsonObject JsonUtils::Merge(const JsonObject& obj1, const JsonObject& obj2,
                            bool overwrite) {
  JsonObject result = obj1;
  for (auto it = obj2.Begin(); it != obj2.End(); ++it) {
    if (overwrite || !result.Contains(it->first)) {
      result.Insert(it->first, it->second);
    }
  }
  return result;
}

std::string JsonUtils::PrettyPrint(const JsonValue& value, int indent) {
  JsonWriterConfig config = JsonWriterConfig::Pretty();
  config.indent_size = indent;
  JsonWriter writer(config);
  return writer.Write(value);
}

std::string JsonUtils::CompactPrint(const JsonValue& value) {
  JsonWriter writer(JsonWriterConfig::Compact());
  return writer.Write(value);
}

std::string JsonUtils::Minify(const std::string& json) {
  try {
    JsonValue value = JsonParser::Parse(json);
    return CompactPrint(value);
  } catch (const JsonException&) {
    return json;  // Return original if parsing fails
  }
}

std::string JsonUtils::Format(const std::string& json, int indent) {
  try {
    JsonValue value = JsonParser::Parse(json);
    return PrettyPrint(value, indent);
  } catch (const JsonException&) {
    return json;  // Return original if parsing fails
  }
}

}  // namespace json_parser

