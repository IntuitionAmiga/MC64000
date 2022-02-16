# Standard Test Host Library

## v1.0.0

## Requirements
This directory contains the stub library includes for the Standard Test Host.

In order to use these libraries, you must specify the Standard Test Host version information in the target section of your project and then include the desired stubs in the sources section of your project file before any files that refer to them:

```javascript
{
    "target": {
        // ...
        "host": {
            "name": "Standard Test Host",
            "version": "<version>"
        }
    },
    ...
    "sources": [
        "lib:standard_test_host/<version>/host.s", // This line is mandatory
        "lib:standard_test_host/<version>/mem.s",  // Include as needed
        "lib:standard_test_host/<version>/io.s",   // Include as needed
        // ...
        // your sources follow
    ]
}
```

### Versioning

Simplistic semantic version is used to check the requirements between the application and host.


### ABI Behaviour

The Host ABI reserves a number of registers for parameters and return.

- r0 and r1 for the first two integer values.
- r8 and r9 for the first two pointer values.
- fp0, fp1 for the first two floating point values.

These are used so that the aliases d0, d1, a0 and a1 retain their historic 680x0 meaning. Generally a host native call does not clobber any registers, however, since the same registers are typically used for return values, these registers should be considered volatile.

Host functions are arranged in up to 256 groups, or vectors, each of which can contain up to 256 operations. Execution of host native code is triggered by a hcf operation which accepts two unsigned byte immediate operands. The first operand is interpreted as the operation selector, the second the vector selector. For simplicity, this library contains stub source files that enumerate these, as well as any commonly used constants, for each library.


For example, in io.s:

```asm

    ; constants
    @equ IO_OPEN_READ  0
    @equ IO_OPEN_WRITE 1

    ; Functions <offset>, <vector>
    @def io_vector #0
    @equ io_init         #0, io_vector
    @equ io_done         #1, io_vector
    @equ io_print_string #2, io_vector
```

Invoking these from your own code is as simple as:

```asm

    @export main x
    @export exit x

main:
    hcf io_init
    lea .hello, r8
    hcf io_print_string
    hcf io_done
exit:
    rts

.hello:
    dc.s "Hello world!\n\0"
```

## Document Example Layout
Each function described is presented in the format shown below.

### function_name
```asm
    ; <reg_name>:<return_type> <return_name> function_name(<reg_name>:<param_type> <param_name>)

    move.q  <param_name>, <reg_name>
    bsr     function_name
```
Description of operation.

- Noteworthy point.
- Since <version>

## Component Reference

- [host.s : Host](docs/host.md)
- [mem.s : Memory](docs/mem.md)
- [io.s : Stream IO](docs/io.md)
