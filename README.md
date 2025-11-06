# Production-Grade C++ JSON Parser

A high-performance, production-ready JSON parser implementation in C++17 following Google C++ Style Guide with comprehensive test coverage and modern design patterns.

## Features

- **Full JSON Support**: Parse and serialize JSON objects, arrays, strings, numbers, booleans, and null values
- **Nested Structures**: Support for deeply nested JSON structures with configurable depth limits
- **Type-Safe API**: Strong typing with compile-time and runtime type checking
- **Design Patterns**: Implements Builder, Visitor, and Strategy patterns
- **Error Handling**: Comprehensive exception hierarchy with detailed error messages
- **Configuration**: Flexible parser and writer configuration options
- **Performance**: Optimized for both parsing and serialization
- **Testing**: Comprehensive test suite with Google Test framework (TDD approach)
- **Google C++ Style**: Follows Google C++ Style Guide conventions

## Architecture

The project is organized into modular components:

- **json_value.h/cpp**: Core JSON value container with type system
- **json_object.h/cpp**: JSON object implementation (key-value pairs)
- **json_array.h/cpp**: JSON array implementation
- **json_parser.h/cpp**: Main parser with configurable parsing strategies
- **json_writer.h/cpp**: JSON serializer with formatting options
- **json_visitor.h/cpp**: Visitor pattern for traversing JSON structures
- **json_builder.h/cpp**: Builder pattern for constructing JSON programmatically
- **json_utils.h/cpp**: Utility functions for common operations
- **json_exception.h**: Custom exception hierarchy

## Design Patterns Used

1. **Builder Pattern**: `JsonBuilder` for fluent JSON construction
2. **Visitor Pattern**: `JsonVisitor` for traversing and processing JSON structures
3. **Strategy Pattern**: Configurable parsing and writing strategies
4. **Factory Pattern**: Static factory methods for common operations

## Building

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.12 or higher
- Google Test (automatically fetched by CMake)

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run tests
ctest --output-on-failure

# Or run tests directly
./json_parser_tests

# Run example
./json_parser_example
```

### Build Options

```bash
# Disable tests
cmake -DBUILD_TESTS=OFF ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Usage

### Basic Parsing

```cpp
#include "json_parser.h"
using namespace json_parser;

// Parse from string
std::string json = R"({"name": "John", "age": 30})";
JsonValue value = JsonParser::Parse(json);

// Access values
std::string name = value.AsObject()["name"].AsString();
double age = value.AsObject()["age"].AsNumber();
```

### Parse from File

```cpp
JsonValue value = JsonParser::ParseFile("data.json");
```

### Using Builder Pattern

```cpp
JsonBuilder builder;
JsonValue json = builder
    .StartObject()
    .Key("user")
    .StartObject()
    .Key("name")
    .Value("Alice")
    .Key("id")
    .Value(12345)
    .EndObject()
    .Key("items")
    .StartArray()
    .AddValue(1)
    .AddValue(2)
    .AddValue(3)
    .EndArray()
    .EndObject()
    .Build();
```

### Using Visitor Pattern

```cpp
class MyVisitor : public JsonVisitor {
    void VisitString(const std::string& str) override {
        // Process string values
    }
    // ... implement other visit methods
};

MyVisitor visitor;
visitor.VisitValue(jsonValue);
```

### Writing JSON

```cpp
JsonWriter writer(JsonWriterConfig::Pretty());
std::string formatted = writer.Write(jsonValue);

// Write to file
writer.WriteToFile("output.json", jsonValue);
```

### Using Utilities

```cpp
// Path-based access
const JsonValue* result = JsonUtils::GetByPath(value, "user.name");

// Type conversions
int age = JsonUtils::AsInt(value.AsObject()["age"], 0);

// Validation
bool valid = JsonUtils::IsValidJson(jsonString);

// Formatting
std::string pretty = JsonUtils::PrettyPrint(value);
std::string compact = JsonUtils::CompactPrint(value);
```

### Configuration

```cpp
// Strict parsing (default)
JsonParserConfig strict = JsonParserConfig::Strict();

// Lenient parsing (allows comments, trailing commas)
JsonParserConfig lenient = JsonParserConfig::Lenient();
lenient.allow_comments = true;
lenient.allow_trailing_commas = true;

JsonValue value = JsonParser::Parse(json, lenient);
```

## Testing

The project includes comprehensive tests following Test-Driven Development (TDD) principles:

- **test_json_value.cpp**: Tests for JSON value types and operations
- **test_json_object.cpp**: Tests for JSON object operations
- **test_json_array.cpp**: Tests for JSON array operations
- **test_json_parser.cpp**: Tests for JSON parsing functionality
- **test_json_writer.cpp**: Tests for JSON serialization
- **test_json_visitor.cpp**: Tests for visitor pattern implementation
- **test_json_builder.cpp**: Tests for builder pattern
- **test_json_utils.cpp**: Tests for utility functions

Run all tests:
```bash
cd build
ctest --output-on-failure
```

## Code Statistics

- **10+ Source Files**: Modular architecture with separation of concerns
- **1000+ Lines of Code**: Comprehensive implementation
- **Design Patterns**: Builder, Visitor, Strategy, Factory
- **Test Coverage**: Comprehensive test suite with Google Test
- **Google C++ Style**: Follows Google C++ Style Guide

## Error Handling

The library provides a comprehensive exception hierarchy:

- `JsonException`: Base exception class
- `JsonParseException`: Parsing errors with position information
- `JsonTypeException`: Type mismatch errors
- `JsonKeyException`: Missing key errors
- `JsonFileException`: File I/O errors

```cpp
try {
    JsonValue value = JsonParser::Parse(json);
} catch (const JsonParseException& e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
} catch (const JsonException& e) {
    std::cerr << "JSON error: " << e.what() << std::endl;
}
```

## Performance Considerations

- Efficient memory management with smart pointers
- Configurable depth and size limits to prevent resource exhaustion
- Optimized string handling and number parsing
- Compact and pretty-print formatting options

## License

This is a production-grade educational implementation. Feel free to use and modify as needed.

## Contributing

When contributing, please ensure:
- Code follows Google C++ Style Guide
- All tests pass
- New features include corresponding tests
- Documentation is updated

## Future Enhancements

- Streaming parser for large JSON files
- JSON Schema validation
- JSONPath query support
- Performance benchmarking suite
- Additional format support (JSON5, JSONC)
