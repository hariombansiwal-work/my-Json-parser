#include "json_parser/json_visitor.h"
#include "json_parser/json_value.h"
#include "json_parser/json_object.h"
#include "json_parser/json_array.h"

namespace json_parser {

void JsonVisitorAdapter::VisitValue(const JsonValue& value) {
  switch (value.GetType()) {
    case JsonValueType::kObject:
      VisitObject(value.AsObject());
      break;
    case JsonValueType::kArray:
      VisitArray(value.AsArray());
      break;
    case JsonValueType::kString:
      VisitString(value.AsString());
      break;
    case JsonValueType::kNumber:
      VisitNumber(value.AsNumber());
      break;
    case JsonValueType::kBoolean:
      VisitBoolean(value.AsBoolean());
      break;
    case JsonValueType::kNull:
      VisitNull();
      break;
  }
}

void JsonVisitorAdapter::VisitObject(const JsonObject& obj) {
  for (auto it = obj.Begin(); it != obj.End(); ++it) {
    VisitValue(it->second);
  }
}

void JsonVisitorAdapter::VisitArray(const JsonArray& arr) {
  for (size_t i = 0; i < arr.Size(); ++i) {
    VisitValue(arr[i]);
  }
}

}  // namespace json_parser

