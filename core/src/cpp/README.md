```
    888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
    8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
    88888b.d88888 888    888 888          d8P 888  888  d8P
    888Y88888P888 888        888d888b.   d8P  888  888d88K
    888 Y888P 888 888        888P "Y88b d88   888  8888888b
    888  Y8P  888 888    888 888    888 8888888888 888  Y88b
    888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
    888       888  "Y8888P"   "Y8888P"        888  888    Y88b

      - 64-bit 680x0-inspired Virtual Machine and Assembler -
```

## Example C++ Runtime

### Standards
- Requires C++17:
    - Shorthand namespaces
    - `if constexpr`
- Does use compile-time checking:
    - static_assert.
    - type checks.
    - range checks.
- Does not focus on memory safety:
    - Primarily this is due to the fact that the virtual machine can construct any plausible 64-bit value and interpret it as an address meaning that it is inherently unsafe.
    - Uses standard C libraries for basic IO and memory allocation.

### Coding Style
- 4 space indentation.
- Opening brace on same line as structure/function/control statement.
- CamelCase for namespace, class, struct, union.
- camelCase for function, method, etc.
- East Const notation:
    - `int const`  for constant integer.
    - `int const*` for pointer to constant integer.
    - `int const*` for constant pointer to integer.
    - `int const* const` for constant pointer to constant integer.
- Simplified Hungarian Notation, single character prefix for non-function identifier names:
    - i for signed integer types int8, int16, int32, int64, short, int, long, etc.
        - int32 iExample
    - u for unsigned integers uint8, uint16, unit32, uint64, unsigned, size_t, etc.
        - size_t uSize
    - e for explicitly enumerated integer values:
        - Interpreter::Status eStatus
    - s for C strings char\*, const char\*.
        - const char\* sMessage
    - o for object/struct immediates:
        - SomeClass oInstance()
        - struct timeval oTime
    - c for callable function pointers, closures, etc.
        - int (\*someFunction)() cCallMe
    - Additional prepend for arrays, pointers and references:
        - ax for arrays of x:
            - int aiHighFive[5]
            - const char* asNames[10]
        - px for pointer to x:
            - SomeClass* poInstance
            - const uint8* puByteCode
            - std::FILE* poStream
        - rx for reference to x:
            - const Host::Definition& roDefinition
