# MASM Sources

This subdirectory contains PHP7.4 sources for the MC64000 _masm_ bytecode assembler. For usage documentation on the _masm_ tool, please refer to [MASM](../docs/assembler/README.md)

## Style

The code largely adheres to common PSR style guidelines, with the following exceptions:

- Opening brace is always placed at the end of the line introducing a block.
- Uses a statically generated classmap autoloader.
- Uses a simple, static Log implementation.
- Variable identifiers use a simplified Hungarian notation style, with a single leading character to indicate the expected type:
    - b for bool
    - i for int
    - f for float
    - s for string
    - a for array
    - o for object
    - r for resorce
    - c for callable
    - m for mixed

## Safety

- All source files declare `strict_types`.
- All function parameters are fully type hinted:
    - Where an array or raw object is passed, a docblock annotation is used to describe the contents.
- All function returns are type hinted:
    - Functions that do not return a value are hinted `void`.
- All class member data are fully type hinted:
    - Where an array or raw object is declared, a docblock annotation us used to describe the contents.
- Code is written for static analysis:
    - Includes `phpstan` configuration.
    - All files under `src/` are tested.
    - Tested using v1.2.0.
    - Does not use any skip-line directives.
    - Level 9 compliance.
- Built-in unit tests:
    - Simple assertion based tests for major components.
    - Each test class runs as a separate process.

## FAQ

### Why PHP?

PHP was chosen for a number of reasons:

- RAD:
    - Familiarity.
    - Very easy to get a working prototype that can be refined into a working application.
    - Write and run workflow. No build process, compilation, etc.
- OOP:
    - The basic design is readily decomposed into well-defined components and related interfaces well suited to Object Oriented programming principles.
    - Good type discipline in PHP7.4 while retaining the sometimes useful ability to use property crates.
- Excellent built-in support for text processing:
    - File I/O.
    - Regular expressions.
    - JSON.
- Adequate performance.

### Why not full PSR?

PSR exists to standardise the coding style used for consistency and interoperability between different PHP frameworks and libraries. Masm is a self-contained application that neither exports or imports code beyond what is commonly included in most PHP CLI runtimes.

### Why not use a standard Unit Testing Framework?

- Most of the assertions required for testing are extremely simplistic, full PHPUnit is surplus to requirements.
- Did not want to introduce external dependencies or deal with composer.
