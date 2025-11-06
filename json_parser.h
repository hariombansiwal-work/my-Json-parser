#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>

namespace json {

// Forward declarations
class JsonValue;
class JsonObject;
class JsonArray;

// JSON value types
using JsonNumber = double;
using JsonString = std::string;
using JsonBool = bool;
using JsonNull = std::nullptr_t;

// JSON value container
class JsonValue {
public:
    enum Type {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        BOOLEAN,
        NULL_VALUE
    };

    JsonValue();
    JsonValue(const JsonObject& obj);
    JsonValue(const JsonArray& arr);
    JsonValue(const std::string& str);
    JsonValue(double num);
    JsonValue(bool b);
    JsonValue(std::nullptr_t);

    Type getType() const { return type_; }
    
    JsonObject& asObject();
    const JsonObject& asObject() const;
    JsonArray& asArray();
    const JsonArray& asArray() const;
    std::string& asString();
    const std::string& asString() const;
    double& asNumber();
    const double& asNumber() const;
    bool& asBoolean();
    const bool& asBoolean() const;

    std::string toString(int indent = 0) const;

private:
    Type type_;
    std::variant<
        std::shared_ptr<JsonObject>,
        std::shared_ptr<JsonArray>,
        std::string,
        double,
        bool,
        std::nullptr_t
    > value_;
};

// JSON Object
class JsonObject {
public:
    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;
    bool has(const std::string& key) const;
    size_t size() const { return values_.size(); }
    void clear() { values_.clear(); }
    
    std::string toString(int indent = 0) const;

private:
    std::unordered_map<std::string, JsonValue> values_;
};

// JSON Array
class JsonArray {
public:
    void push_back(const JsonValue& value);
    JsonValue& operator[](size_t index);
    const JsonValue& operator[](size_t index) const;
    size_t size() const { return values_.size(); }
    void clear() { values_.clear(); }
    
    std::string toString(int indent = 0) const;

private:
    std::vector<JsonValue> values_;
};

// JSON Parser
class JsonParser {
public:
    static JsonValue parse(const std::string& json);
    static JsonValue parseFile(const std::string& filename);

private:
    std::string input_;
    size_t pos_;

    JsonParser(const std::string& input);
    JsonValue parseValue();
    JsonObject parseObject();
    JsonArray parseArray();
    std::string parseString();
    double parseNumber();
    bool parseBoolean();
    void parseNull();
    void skipWhitespace();
    char current() const;
    char next();
    bool expect(char c);
};

} // namespace json

#endif // JSON_PARSER_H

