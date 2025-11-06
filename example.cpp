#include "json_parser.h"
#include <iostream>
#include <exception>

using namespace json;

int main() {
    try {
        // Example 1: Parse a simple JSON string
        std::cout << "=== Example 1: Simple JSON Object ===\n";
        std::string json1 = R"({
            "name": "John Doe",
            "age": 30,
            "city": "New York",
            "isActive": true
        })";
        
        JsonValue value1 = JsonParser::parse(json1);
        std::cout << "Parsed JSON:\n" << value1.toString() << "\n\n";
        
        // Access values
        std::cout << "Name: " << value1.asObject()["name"].asString() << "\n";
        std::cout << "Age: " << value1.asObject()["age"].asNumber() << "\n";
        std::cout << "City: " << value1.asObject()["city"].asString() << "\n";
        std::cout << "Is Active: " << (value1.asObject()["isActive"].asBoolean() ? "true" : "false") << "\n\n";
        
        // Example 2: Parse JSON array
        std::cout << "=== Example 2: JSON Array ===\n";
        std::string json2 = R"([1, 2, 3, "hello", true, null])";
        
        JsonValue value2 = JsonParser::parse(json2);
        std::cout << "Parsed JSON:\n" << value2.toString() << "\n\n";
        
        // Access array elements
        std::cout << "Array size: " << value2.asArray().size() << "\n";
        std::cout << "First element: " << value2.asArray()[0].toString() << "\n";
        std::cout << "Fourth element: " << value2.asArray()[3].toString() << "\n\n";
        
        // Example 3: Nested JSON
        std::cout << "=== Example 3: Nested JSON ===\n";
        std::string json3 = R"({
            "user": {
                "id": 12345,
                "name": "Alice",
                "preferences": {
                    "theme": "dark",
                    "notifications": true
                }
            },
            "items": [
                {"id": 1, "name": "Item 1"},
                {"id": 2, "name": "Item 2"}
            ],
            "metadata": null
        })";
        
        JsonValue value3 = JsonParser::parse(json3);
        std::cout << "Parsed JSON:\n" << value3.toString() << "\n\n";
        
        // Access nested values
        std::cout << "User ID: " << value3.asObject()["user"].asObject()["id"].asNumber() << "\n";
        std::cout << "User Name: " << value3.asObject()["user"].asObject()["name"].asString() << "\n";
        std::cout << "Theme: " << value3.asObject()["user"].asObject()["preferences"].asObject()["theme"].asString() << "\n";
        std::cout << "First item name: " << value3.asObject()["items"].asArray()[0].asObject()["name"].asString() << "\n\n";
        
        // Example 4: Numbers
        std::cout << "=== Example 4: Numbers ===\n";
        std::string json4 = R"({
            "integer": 42,
            "float": 3.14159,
            "negative": -10,
            "scientific": 1.5e3
        })";
        
        JsonValue value4 = JsonParser::parse(json4);
        std::cout << "Parsed JSON:\n" << value4.toString() << "\n\n";
        
        std::cout << "All examples completed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

