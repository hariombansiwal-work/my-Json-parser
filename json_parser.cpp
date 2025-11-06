#include "json_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iomanip>
#include <cmath>

namespace json {

// JsonValue implementation
JsonValue::JsonValue() : type_(NULL_VALUE), value_(nullptr) {}

JsonValue::JsonValue(const JsonObject& obj) 
    : type_(OBJECT), value_(std::make_shared<JsonObject>(obj)) {}

JsonValue::JsonValue(const JsonArray& arr) 
    : type_(ARRAY), value_(std::make_shared<JsonArray>(arr)) {}

JsonValue::JsonValue(const std::string& str) 
    : type_(STRING), value_(str) {}

JsonValue::JsonValue(double num) 
    : type_(NUMBER), value_(num) {}

JsonValue::JsonValue(bool b) 
    : type_(BOOLEAN), value_(b) {}

JsonValue::JsonValue(std::nullptr_t) 
    : type_(NULL_VALUE), value_(nullptr) {}

JsonObject& JsonValue::asObject() {
    if (type_ != OBJECT) throw std::runtime_error("Not an object");
    return *std::get<std::shared_ptr<JsonObject>>(value_);
}

const JsonObject& JsonValue::asObject() const {
    if (type_ != OBJECT) throw std::runtime_error("Not an object");
    return *std::get<std::shared_ptr<JsonObject>>(value_);
}

JsonArray& JsonValue::asArray() {
    if (type_ != ARRAY) throw std::runtime_error("Not an array");
    return *std::get<std::shared_ptr<JsonArray>>(value_);
}

const JsonArray& JsonValue::asArray() const {
    if (type_ != ARRAY) throw std::runtime_error("Not an array");
    return *std::get<std::shared_ptr<JsonArray>>(value_);
}

std::string& JsonValue::asString() {
    if (type_ != STRING) throw std::runtime_error("Not a string");
    return std::get<std::string>(value_);
}

const std::string& JsonValue::asString() const {
    if (type_ != STRING) throw std::runtime_error("Not a string");
    return std::get<std::string>(value_);
}

double& JsonValue::asNumber() {
    if (type_ != NUMBER) throw std::runtime_error("Not a number");
    return std::get<double>(value_);
}

const double& JsonValue::asNumber() const {
    if (type_ != NUMBER) throw std::runtime_error("Not a number");
    return std::get<double>(value_);
}

bool& JsonValue::asBoolean() {
    if (type_ != BOOLEAN) throw std::runtime_error("Not a boolean");
    return std::get<bool>(value_);
}

const bool& JsonValue::asBoolean() const {
    if (type_ != BOOLEAN) throw std::runtime_error("Not a boolean");
    return std::get<bool>(value_);
}

std::string JsonValue::toString(int indent) const {
    switch (type_) {
        case OBJECT:
            return std::get<std::shared_ptr<JsonObject>>(value_)->toString(indent);
        case ARRAY:
            return std::get<std::shared_ptr<JsonArray>>(value_)->toString(indent);
        case STRING:
            return "\"" + std::get<std::string>(value_) + "\"";
        case NUMBER:
            {
                std::ostringstream oss;
                double num = std::get<double>(value_);
                if (num == static_cast<long long>(num)) {
                    oss << static_cast<long long>(num);
                } else {
                    oss << std::fixed << std::setprecision(6) << num;
                }
                return oss.str();
            }
        case BOOLEAN:
            return std::get<bool>(value_) ? "true" : "false";
        case NULL_VALUE:
            return "null";
    }
    return "null";
}

// JsonObject implementation
JsonValue& JsonObject::operator[](const std::string& key) {
    return values_[key];
}

const JsonValue& JsonObject::operator[](const std::string& key) const {
    auto it = values_.find(key);
    if (it == values_.end()) {
        throw std::runtime_error("Key not found: " + key);
    }
    return it->second;
}

bool JsonObject::has(const std::string& key) const {
    return values_.find(key) != values_.end();
}

std::string JsonObject::toString(int indent) const {
    if (values_.empty()) return "{}";
    
    std::string result = "{\n";
    std::string indentStr(indent + 2, ' ');
    bool first = true;
    
    for (const auto& pair : values_) {
        if (!first) result += ",\n";
        first = false;
        result += indentStr + "\"" + pair.first + "\": " + pair.second.toString(indent + 2);
    }
    
    result += "\n" + std::string(indent, ' ') + "}";
    return result;
}

// JsonArray implementation
void JsonArray::push_back(const JsonValue& value) {
    values_.push_back(value);
}

JsonValue& JsonArray::operator[](size_t index) {
    if (index >= values_.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    return values_[index];
}

const JsonValue& JsonArray::operator[](size_t index) const {
    if (index >= values_.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    return values_[index];
}

std::string JsonArray::toString(int indent) const {
    if (values_.empty()) return "[]";
    
    std::string result = "[\n";
    std::string indentStr(indent + 2, ' ');
    
    for (size_t i = 0; i < values_.size(); ++i) {
        if (i > 0) result += ",\n";
        result += indentStr + values_[i].toString(indent + 2);
    }
    
    result += "\n" + std::string(indent, ' ') + "]";
    return result;
}

// JsonParser implementation
JsonValue JsonParser::parse(const std::string& json) {
    JsonParser parser(json);
    parser.skipWhitespace();
    JsonValue result = parser.parseValue();
    parser.skipWhitespace();
    if (parser.pos_ < parser.input_.length()) {
        throw std::runtime_error("Unexpected characters after JSON value");
    }
    return result;
}

JsonValue JsonParser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return parse(buffer.str());
}

JsonParser::JsonParser(const std::string& input) : input_(input), pos_(0) {}

JsonValue JsonParser::parseValue() {
    skipWhitespace();
    char c = current();
    
    if (c == '{') {
        return JsonValue(parseObject());
    } else if (c == '[') {
        return JsonValue(parseArray());
    } else if (c == '"') {
        return JsonValue(parseString());
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        return JsonValue(parseNumber());
    } else if (c == 't' || c == 'f') {
        return JsonValue(parseBoolean());
    } else if (c == 'n') {
        parseNull();
        return JsonValue(nullptr);
    } else {
        throw std::runtime_error("Unexpected character: " + std::string(1, c));
    }
}

JsonObject JsonParser::parseObject() {
    JsonObject obj;
    expect('{');
    skipWhitespace();
    
    if (current() == '}') {
        next();
        return obj;
    }
    
    while (true) {
        skipWhitespace();
        std::string key = parseString();
        skipWhitespace();
        expect(':');
        skipWhitespace();
        JsonValue value = parseValue();
        obj[key] = value;
        skipWhitespace();
        
        char c = current();
        if (c == '}') {
            next();
            break;
        } else if (c == ',') {
            next();
        } else {
            throw std::runtime_error("Expected ',' or '}' in object");
        }
    }
    
    return obj;
}

JsonArray JsonParser::parseArray() {
    JsonArray arr;
    expect('[');
    skipWhitespace();
    
    if (current() == ']') {
        next();
        return arr;
    }
    
    while (true) {
        skipWhitespace();
        JsonValue value = parseValue();
        arr.push_back(value);
        skipWhitespace();
        
        char c = current();
        if (c == ']') {
            next();
            break;
        } else if (c == ',') {
            next();
        } else {
            throw std::runtime_error("Expected ',' or ']' in array");
        }
    }
    
    return arr;
}

std::string JsonParser::parseString() {
    expect('"');
    std::string result;
    
    while (pos_ < input_.length()) {
        char c = next();
        if (c == '"') {
            break;
        } else if (c == '\\') {
            char esc = next();
            switch (esc) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                case 'u': {
                    // Simple Unicode escape - just skip for now
                    for (int i = 0; i < 4; ++i) next();
                    break;
                }
                default: result += esc; break;
            }
        } else {
            result += c;
        }
    }
    
    return result;
}

double JsonParser::parseNumber() {
    bool negative = false;
    
    if (current() == '-') {
        negative = true;
        next();
    }
    
    double result = 0.0;
    while (pos_ < input_.length() && std::isdigit(current())) {
        result = result * 10 + (current() - '0');
        next();
    }
    
    if (current() == '.') {
        next();
        double fraction = 0.0;
        double divisor = 10.0;
        while (pos_ < input_.length() && std::isdigit(current())) {
            fraction += (current() - '0') / divisor;
            divisor *= 10.0;
            next();
        }
        result += fraction;
    }
    
    if (current() == 'e' || current() == 'E') {
        next();
        bool expNegative = false;
        if (current() == '-') {
            expNegative = true;
            next();
        } else if (current() == '+') {
            next();
        }
        
        int exponent = 0;
        while (pos_ < input_.length() && std::isdigit(current())) {
            exponent = exponent * 10 + (current() - '0');
            next();
        }
        
        if (expNegative) exponent = -exponent;
        result *= std::pow(10.0, exponent);
    }
    
    return negative ? -result : result;
}

bool JsonParser::parseBoolean() {
    if (input_.substr(pos_, 4) == "true") {
        pos_ += 4;
        return true;
    } else if (input_.substr(pos_, 5) == "false") {
        pos_ += 5;
        return false;
    } else {
        throw std::runtime_error("Invalid boolean value");
    }
}

void JsonParser::parseNull() {
    if (input_.substr(pos_, 4) == "null") {
        pos_ += 4;
    } else {
        throw std::runtime_error("Invalid null value");
    }
}

void JsonParser::skipWhitespace() {
    while (pos_ < input_.length() && std::isspace(input_[pos_])) {
        pos_++;
    }
}

char JsonParser::current() const {
    if (pos_ >= input_.length()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return input_[pos_];
}

char JsonParser::next() {
    if (pos_ >= input_.length()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return input_[pos_++];
}

bool JsonParser::expect(char c) {
    if (current() != c) {
        throw std::runtime_error("Expected '" + std::string(1, c) + "' but got '" + std::string(1, current()) + "'");
    }
    next();
    return true;
}

} // namespace json

