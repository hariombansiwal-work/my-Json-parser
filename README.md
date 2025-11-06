# C++ JSON Parser

A simple, header-only JSON parser implementation in C++17.

## Features

- Parse JSON objects, arrays, strings, numbers, booleans, and null values
- Support for nested JSON structures
- Type-safe access to parsed values
- Pretty-printing of JSON values
- Parse from strings or files
- Error handling with descriptive messages

## Building

To build the example program:

```bash
make
```

To clean build artifacts:

```bash
make clean
```

## Usage

### Basic Example

```cpp
#include "json_parser.h"
#include <iostream>

using namespace json;

int main() {
    std::string json = R"({
        "name": "John Doe",
        "age": 30,
        "city": "New York"
    })";
    
    JsonValue value = JsonParser::parse(json);
    
    // Access values
    std::cout << value.asObject()["name"].asString() << std::endl;
    std::cout << value.asObject()["age"].asNumber() << std::endl;
    
    return 0;
}
```

### Parse from File

```cpp
JsonValue value = JsonParser::parseFile("data.json");
```

### Working with Arrays

```cpp
std::string json = R"([1, 2, 3, "hello"])";
JsonValue arr = JsonParser::parse(json);

for (size_t i = 0; i < arr.asArray().size(); ++i) {
    std::cout << arr.asArray()[i].toString() << std::endl;
}
```

### Nested Objects

```cpp
std::string json = R"({
    "user": {
        "name": "Alice",
        "id": 12345
    }
})";

JsonValue root = JsonParser::parse(json);
std::string name = root.asObject()["user"].asObject()["name"].asString();
```

## Running the Example

```bash
./json_parser_example
```

## Requirements

- C++17 or later
- GCC or Clang compiler

## License

This is a simple educational implementation. Feel free to use and modify as needed.
