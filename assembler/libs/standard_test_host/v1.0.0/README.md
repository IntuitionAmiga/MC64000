# Standard Test Host Library

## v1.0.0

## Requirements
This directory contains the stub library includes for the Standard Test Host.

In order to use these libraries, you must specify the Standard Test Host version information in the target section of your project and then include the desired stubs in the sources section of your project file before any files that refer to them:

```json
    "target": {
        ...
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
        ...
        // your sources follow
    ]
```

### Versioning

Simplistic semantic version is used to check the requirements between the application and host.


### ABI Behaviour

The Host ABI reserves a number of registers for parameters and return.

- r0 and r1 for the first two integer values.
- r8 and r9 for the first two pointer values.
- fp0, fp1 for the first two floating point values.

These are used so that the aliases d0, d1, a0 and a1 retain their historic 680x0 meaning. Generally a host native call does not clobber any registers, however, since the same registers are typically used for return values, these registers should be considered volatile.

Execution of host native code is triggered by a hcf operation. MC64K allows up to 256 host vectors. Rather than assign every function to a host vector, functions are grouped into related sets, each of which has a single vector. Within each vector, functions are assigned an increnting ID value. To invoke a specific function, it's ID is first pushed onto the stack and then the vector to which it belongs is triggered:

```asm

    @def EXAMPLE_HOST_VECTOR 0
    @def EXAMPLE_HOST_FUNCTION_ID 0

example_host_function:
    move.b  #EXAMPLE_HOST_FUNCTION_ID, -(sp) ; load the function ID onto the stack
    hcf     #EXAMPLE_HOST_VECTOR             ; trigger the vector
    add.q   #1, sp                           ; restore the stack
    rts                                      ; done
```

In order to reduce code size, the included libary files share the trigger and stack restore logic across multiple functions, e.g:

```asm

.invoke:
    hcf     #HOST_VECTOR
    add.q   #1, sp
    rts

example_host_function_1:
    move.b  #EXAMPLE_HOST_FUNCTION_1_ID, -(sp)
    bra.b   .invoke

example_host_function_2:
    move.b  #EXAMPLE_HOST_FUNCTION_2_ID, -(sp)
    bra.b   .invoke

example_host_function_3:
    move.b  #EXAMPLE_HOST_FUNCTION_3_ID, -(sp)
    bra.b   .invoke
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
