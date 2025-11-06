#ifndef JSON_PARSER_JSON_VISITOR_H_
#define JSON_PARSER_JSON_VISITOR_H_

#include <string>

// Visitor pattern for traversing JSON structures
namespace json_parser {

class JsonValue;
class JsonObject;
class JsonArray;

// Abstract visitor interface
class JsonVisitor {
 public:
  virtual ~JsonVisitor() = default;

  // Visit methods for different JSON types
  virtual void VisitValue(const JsonValue& value) = 0;
  virtual void VisitObject(const JsonObject& obj) = 0;
  virtual void VisitArray(const JsonArray& arr) = 0;
  virtual void VisitString(const std::string& str) = 0;
  virtual void VisitNumber(double num) = 0;
  virtual void VisitBoolean(bool b) = 0;
  virtual void VisitNull() = 0;
};

// Abstract visitor that provides default implementations
class JsonVisitorAdapter : public JsonVisitor {
 public:
  ~JsonVisitorAdapter() override = default;

  void VisitValue(const JsonValue& value) override;
  void VisitObject(const JsonObject& obj) override;
  void VisitArray(const JsonArray& arr) override;
  void VisitString(const std::string& /* str */) override {}
  void VisitNumber(double /* num */) override {}
  void VisitBoolean(bool /* b */) override {}
  void VisitNull() override {}
};

}  // namespace json_parser

#endif  // JSON_PARSER_JSON_VISITOR_H_

