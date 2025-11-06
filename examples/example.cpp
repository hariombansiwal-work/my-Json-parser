#include "json_parser.h"
#include <iostream>
#include <exception>

using namespace json_parser;

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

    JsonValue value1 = JsonParser::Parse(json1);
    std::cout << "Parsed JSON:\n" << value1.ToString() << "\n\n";

    // Access values
    std::cout << "Name: " << value1.AsObject()["name"].AsString() << "\n";
    std::cout << "Age: " << value1.AsObject()["age"].AsNumber() << "\n";
    std::cout << "City: " << value1.AsObject()["city"].AsString() << "\n";
    std::cout << "Is Active: "
              << (value1.AsObject()["isActive"].AsBoolean() ? "true" : "false")
              << "\n\n";

    // Example 2: Parse JSON array
    std::cout << "=== Example 2: JSON Array ===\n";
    std::string json2 = R"([1, 2, 3, "hello", true, null])";

    JsonValue value2 = JsonParser::Parse(json2);
    std::cout << "Parsed JSON:\n" << value2.ToString() << "\n\n";

    // Access array elements
    std::cout << "Array size: " << value2.AsArray().Size() << "\n";
    std::cout << "First element: " << value2.AsArray()[0].ToString() << "\n";
    std::cout << "Fourth element: " << value2.AsArray()[3].ToString()
              << "\n\n";

    // Example 3: Using JsonBuilder
    std::cout << "=== Example 3: Using JsonBuilder ===\n";
    JsonBuilder builder;
    builder.StartObject()
        .Key("user")
        .StartObject()
        .Key("id")
        .Value(12345)
        .Key("name")
        .Value("Alice")
        .EndObject()
        .Key("items")
        .StartArray()
        .AddValue(1)
        .AddValue(2)
        .AddValue(3)
        .EndArray()
        .EndObject();

    JsonValue value3 = builder.Build();
    std::cout << "Built JSON:\n" << value3.ToString() << "\n\n";

    // Example 4: Using JsonWriter
    std::cout << "=== Example 4: Using JsonWriter ===\n";
    JsonWriter writer(JsonWriterConfig::Pretty());
    std::string formatted = writer.Write(value3);
    std::cout << "Formatted JSON:\n" << formatted << "\n\n";

    // Example 5: Using JsonUtils
    std::cout << "=== Example 5: Using JsonUtils ===\n";
    std::string path_result = JsonUtils::AsString(
        *JsonUtils::GetByPath(value3, "user.name"), "not found");
    std::cout << "Path 'user.name': " << path_result << "\n\n";

    std::cout << "All examples completed successfully!\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}

