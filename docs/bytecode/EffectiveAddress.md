## Bytecode Format > Effective Address

The bytecode formats for the suppored addressing modes are documented here.

One byte is used for the Effective Address Mode of an operand:

* The simplest addressing modes do not require any additional bytes.
* Fixed 32-bit displacements require an additional 4 bytes to represent the displacement value.
* Indexed addressing modes encode the index size and scale in the EA byte and the register pair in one additional byte.
* Integer Immediates require as many additional bytes as are required by the operation size.
* Absolute Address Immediates require an additional 2, 4 or 8 bytes, depending on the word size.

### General Purpose Register Direct

`r<N>`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `r0` | 0x00 |
| `r1` | 0x01 |
| ... | ... |
| `r15` | 0x0F |

### General Purpose Register Indirect

`(r<N>)`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `(r0)` | 0x10 |
| `(r1)` | 0x11 |
| ... | ... |
| `(r15)` | 0x1F |

### General Purpose Register Indirect, Post Increment

`(r<N>)+`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `(r0)+` | 0x20 |
| `(r1)+` | 0x21 |
| ... | ... |
| `(r15)+` | 0x2F |

### General Purpose Register Indirect, Post Decrement

`(r<N>)-`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `(r0)-` | 0x30 |
| `(r1)-` | 0x31 |
| ... | ... |
| `(r15)-` | 0x3F |

### General Purpose Register Indirect, Pre Increment

`+(r<N>)`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `+(r0)` | 0x40 |
| `+(r1)` | 0x41 |
| ... | ... |
| `+(r15)` | 0x4F |

### General Purpose Register Indirect, Pre Decrement

`-(r<N>)`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `-(r0)` | 0x50 |
| `-(r1)` | 0x51 |
| ... | ... |
| `-(r15)` | 0x5F |


### General Purpose Register Indirect with Displacement

`<D>(r<N>)`

* N = 0 ... 15
* D = -2147483648 ... 2147483647

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `(r0)` | 0x60 | 0xDD | 0xDD | 0xDD | 0xDD |
| `(r1)` | 0x61 | 0xDD | 0xDD | 0xDD | 0xDD |
| ... | ... | ... | ... | ... | ... |
| `(r15)` | 0x6F | 0xDD | 0xDD | 0xDD | 0xDD |


### Floating Point Register Direct

`fp<N>`

* N = 0 ... 15

| Mode | Bytecode |
| - | - |
| `fp0` | 0x70 |
| `fp1` | 0x71 |
| ... | ... |
| `fp15` | 0x7F |

### General Purpose Register Indirect with (Scaled) Index

`(r<A>, r<I>.<b|w|l|q> [* <2|4|8>])`

* A = 0 ... 15
* I = 0 ... 15

| Mode | Bytecode | Ext 0 |
| - | - | - |
| `(r<A>, r<I>.b)` | 0x80 | 0x(A)(I) |
| `(r<A>, r<I>.w)` | 0x81 | 0x(A)(I) |
| `(r<A>, r<I>.l)` | 0x82 | 0x(A)(I) |
| `(r<A>, r<I>.q)` | 0x83 | 0x(A)(I) |
| `(r<A>, r<I>.b * 2)` | 0x84 | 0x(A)(I) |
| `(r<A>, r<I>.w * 2)` | 0x85 | 0x(A)(I) |
| `(r<A>, r<I>.l * 2)` | 0x86 | 0x(A)(I) |
| `(r<A>, r<I>.q * 2)` | 0x87 | 0x(A)(I) |
| `(r<A>, r<I>.b * 4)` | 0x88 | 0x(A)(I) |
| `(r<A>, r<I>.w * 4)` | 0x89 | 0x(A)(I) |
| `(r<A>, r<I>.l * 4)` | 0x8A | 0x(A)(I) |
| `(r<A>, r<I>.q * 4)` | 0x8B | 0x(A)(I) |
| `(r<A>, r<I>.b * 8)` | 0x8C | 0x(A)(I) |
| `(r<A>, r<I>.w * 8)` | 0x8D | 0x(A)(I) |
| `(r<A>, r<I>.l * 8)` | 0x8E | 0x(A)(I) |
| `(r<A>, r<I>.q * 8)` | 0x8F | 0x(A)(I) |

### General Purpose Register Indirect with (Scaled) Index and Displacement

`<D>(r<A>, r<I>.<b|w|l|q> [ * <2|4|8>])`

* A = 0 ... 15
* I = 0 ... 15
* D = -2147483648 ... 2147483647

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 | Ext 4 |
| - | - | - | - | - | - | - |
| `<D>(r<A>, r<I>.b)` | 0x90 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.w)` | 0x91 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.l)` | 0x92 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.q)` | 0x93 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.b * 2)` | 0x94 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.w * 2)` | 0x95 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.l * 2)` | 0x96 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.q * 2)` | 0x97 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.b * 4)` | 0x98 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.w * 4)` | 0x99 | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.l * 4)` | 0x9A | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.q * 4)` | 0x9B | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.b * 8)` | 0x9C | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.w * 8)` | 0x9D | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.l * 8)` | 0x9E | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(r<A>, r<I>.q * 8)` | 0x9F | 0x(A)(I) | 0xDD | 0xDD | 0xDD | 0xDD |

### Program Counter Indirect with (Scaled) Index

`(pc, r<I>.<b|w|l|q> [* <2|4|8>])`

* I = 0 ... 15


| Mode | Bytecode | Ext 0 |
| - | - | - |
| `(pc, r<I>.b)` | 0xA0 | 0x0(I) |
| `(pc, r<I>.w)` | 0xA1 | 0x0(I) |
| `(pc, r<I>.l)` | 0xA2 | 0x0(I) |
| `(pc, r<I>.q)` | 0xA3 | 0x0(I) |
| `(pc, r<I>.b * 2)` | 0xA4 | 0x0(I) |
| `(pc, r<I>.w * 2)` | 0xA5 | 0x0(I) |
| `(pc, r<I>.l * 2)` | 0xA6 | 0x0(I) |
| `(pc, r<I>.q * 2)` | 0xA7 | 0x0(I) |
| `(pc, r<I>.b * 4)` | 0xA8 | 0x0(I) |
| `(pc, r<I>.w * 4)` | 0xA9 | 0x0(I) |
| `(pc, r<I>.l * 4)` | 0xAA | 0x0(I) |
| `(pc, r<I>.q * 4)` | 0xAB | 0x0(I) |
| `(pc, r<I>.b * 8)` | 0xAC | 0x0(I) |
| `(pc, r<I>.w * 8)` | 0xAD | 0x0(I) |
| `(pc, r<I>.l * 8)` | 0xAE | 0x0(I) |
| `(pc, r<I>.q * 8)` | 0xAF | 0x0(I) |

### Program Counter Indirect with (Scaled) Index and Displacement

`<D>(pc, r<I>.<b|w|l|q> [* <2|4|8>])`

* I = 0 ... 15
* D = -2147483648 ... 2147483647

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 | Ext 4 |
| - | - | - | - | - | - | - |
| `<D>(pc, r<I>.b)` | 0xB0 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.w)` | 0xB1 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.l)` | 0xB2 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.q)` | 0xB3 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.b * 2)` | 0xB4 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.w * 2)` | 0xB5 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.l * 2)` | 0xB6 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.q * 2)` | 0xB7 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.b * 4)` | 0xB8 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.w * 4)` | 0xB9 | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.l * 4)` | 0xBA | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.q * 4)` | 0xBB | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.b * 8)` | 0xBC | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.w * 8)` | 0xBD | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.l * 8)` | 0xBE | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |
| `<D>(pc, r<I>.q * 8)` | 0xBF | 0x0(I) | 0xDD | 0xDD | 0xDD | 0xDD |

### Program Indirect Counter with Displacement

`<D>(pc)`

* D = -2147483648 ... 2147483647

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `<D>(pc)` | 0xC0 | 0xDD | 0xDD | 0xDD | 0xDD |

### Integer Immediate

`#<D>`

* Instruction determines size.
* D(b) = -128 ... 127
* D(w) = -32768 ... 32767
* D(l) = -2147483648 ... 2147483647
* D(q) = -9223372036854775808 ... 9223372036854775807

| Mode | Bytecode | Ext 0 | ...  | Ext (_Operation Size - 1_) |
| - | - | - | - | - |
| `#<D>` | 0xC1 | 0xDD | ... | 0xDD |

### Same As Destination

* Source Operand Only.
* Used where the Source Effective Address mode is the same as the Destination Effective Address mode and that mode does not have any increment/decrement behaviour.
* Bytecode / decode logic optimisation.

| Mode | Bytecode |
| - | - |
|   | 0xC2 |
