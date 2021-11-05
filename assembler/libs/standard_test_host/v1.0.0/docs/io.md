[Standard Test Host Library](../README.md)

# IO Functions

## io.s
Provides functions for basic stream IO.

### io_init
void io_init()
```asm
    bsr io_init
```
Initialises the host stream IO subsystem. Should be called from _main_ before any other stream IO functions.

- Since v1.0.0
___

### io_done
void io_done()
```asm
    bsr io_done
```
Finalises the host stream IO subsytem. Should be called from _exit_ after all other stream IO functions.

- Since v1.0.0
___

### [Terminal Output](./io_stdout.md)
Basic terminal output routines.

### [Default Format Control](./io_format.md)
Setting the default text formatting options for scalar output and input.

### [File Input/Output](./io_file.md)
Basic file stream output and input.

### [Character Buffer Input/Output](./io_buffer.md)
Basic scalar to string conversion.
