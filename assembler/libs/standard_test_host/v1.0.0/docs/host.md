# Standard Test Host Library

[Main Index](../README.md)

## host.s
Host specific definitions, imports and exports.

### host_info
```asm
    @import host_info r

    ; example usage, report host information
    lea     host_info, a0
    hcf     io_print_string
```
Readonly string description of the Host.

- Since v1.0.0
___

### host_cli_num_params
```asm
    @import host_cli_num_params r

    ; example usage, display parameter count
    move.q  host_cli_num_params, d0
    hcf     io_print_quad
```
Readonly uint64 count of the number of commandline parameters passed to the host on startup.

- Copy of the host argc.
- Since v1.0.0
___

### host_cli_params
```asm
    @import host_cli_params r

    ; example usage, display first parameter
    move.q  host_cli_params, a2
    move.q  (a2), a0
    hcf     io_print_string
```
Readonly char const** list of the commandline parameters passed to the host on startup.

- Maps to the host argv.
- Since v1.0.0
___

### main
```asm
    @export main x
```
Main entry point for the mc64k binary. The host expects to be able to execute this entry point on startup.

- Binary must define `main` as an executable label.
- Since v1.0.0
___

### exit
```asm
    @export exit x
```
Main exit point for the mc64k binary. The host expects to be able to execute this entry point on shutdown.

- Binary must define `exit` as an executable label.
- Since v1.0.0
___
