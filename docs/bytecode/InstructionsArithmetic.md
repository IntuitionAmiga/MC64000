## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Arithmetic Group

The bytecode formats for the suppored arithmetic instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

___

### EXTB

Sign Extend Byte

        <ea(s)> -> <ea(d)>

`extb.<w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `extb.w <ea(s)>, <ea(d)>` | 0xA0 | 0xEA(d) | ... | 0xEA(s) | ... |
| `extb.l <ea(s)>, <ea(d)>` | 0xA1 | 0xEA(d) | ... | 0xEA(s) | ... |
| `extb.q <ea(s)>, <ea(d)>` | 0xA2 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### EXTW

Sign Extend Word

        <ea(s)> -> <ea(d)>

`extw.<l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `extw.l <ea(s)>, <ea(d)>` | 0xA3 | 0xEA(d) | ... | 0xEA(s) | ... |
| `extw.q <ea(s)>, <ea(d)>` | 0xA4 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### EXTL

Sign Extend Long

        <ea(s)> -> <ea(d)>

`extl.q <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `extl.q <ea(s)>, <ea(d)>` | 0xA5 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### ASL

Arithmetic Shift Left

        <ea(d)> <<@ <ea(s)> -> <ea(d)>

`asl.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `asl.b <ea(s)>, <ea(d)>` | 0xA6 | 0xEA(d) | ... | 0xEA(s) | ... |
| `asl.w <ea(s)>, <ea(d)>` | 0xA7 | 0xEA(d) | ... | 0xEA(s) | ... |
| `asl.l <ea(s)>, <ea(d)>` | 0xA8 | 0xEA(d) | ... | 0xEA(s) | ... |
| `asl.q <ea(s)>, <ea(d)>` | 0xA9 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### ASR

Arithmetic Shift Right

        <ea(d)> @>> <ea(s)> -> <ea(d)>

`asr.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `asr.b <ea(s)>, <ea(d)>` | 0xAA | 0xEA(d) | ... | 0xEA(s) | ... |
| `asr.w <ea(s)>, <ea(d)>` | 0xAB | 0xEA(d) | ... | 0xEA(s) | ... |
| `asr.l <ea(s)>, <ea(d)>` | 0xAC | 0xEA(d) | ... | 0xEA(s) | ... |
| `asr.q <ea(s)>, <ea(d)>` | 0xAD | 0xEA(d) | ... | 0xEA(s) | ... |

___

### ADD

Add source to destination

        <ea(d)> + <ea(s)> -> <ea(d)>

`add.<b|w|l|q> <ea(s)>, <ea(d)>`

`fadd.<s|d> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `add.b <ea(s)>, <ea(d)>` | 0xAE | 0xEA(d) | ... | 0xEA(s) | ... |
| `add.w <ea(s)>, <ea(d)>` | 0xAF | 0xEA(d) | ... | 0xEA(s) | ... |
| `add.l <ea(s)>, <ea(d)>` | 0xB0 | 0xEA(d) | ... | 0xEA(s) | ... |
| `add.q <ea(s)>, <ea(d)>` | 0xB1 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fadd.s <ea(s)>, <ea(d)>` | 0xB2 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fadd.d <ea(s)>, <ea(d)>` | 0xB3 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### SUB

Subtract source from destination

        <ea(d)> - <ea(s)> -> <ea(d)>

`sub.<b|w|l|q> <ea(s)>, <ea(d)>`

`fsub.<s|d> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `sub.b <ea(s)>, <ea(d)>` | 0xB4 | 0xEA(d) | ... | 0xEA(s) | ... |
| `sub.w <ea(s)>, <ea(d)>` | 0xB5 | 0xEA(d) | ... | 0xEA(s) | ... |
| `sub.l <ea(s)>, <ea(d)>` | 0xB6 | 0xEA(d) | ... | 0xEA(s) | ... |
| `sub.q <ea(s)>, <ea(d)>` | 0xB7 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fsub.s <ea(s)>, <ea(d)>` | 0xB8 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fsub.d <ea(s)>, <ea(d)>` | 0xB9 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### NEG

Negate Source to Destination

         - <ea(s)> -> <ea(d)>

`neg.<b|w|l|q> <ea(s)>, <ea(d)>`

`fneg.<s|d> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `neg.b <ea(s)>, <ea(d)>` | 0xBA | 0xEA(d) | ... | 0xEA(s) | ... |
| `neg.w <ea(s)>, <ea(d)>` | 0xBB | 0xEA(d) | ... | 0xEA(s) | ... |
| `neg.l <ea(s)>, <ea(d)>` | 0xBC | 0xEA(d) | ... | 0xEA(s) | ... |
| `neg.q <ea(s)>, <ea(d)>` | 0xBD | 0xEA(d) | ... | 0xEA(s) | ... |
| `fneg.s <ea(s)>, <ea(d)>` | 0xBE | 0xEA(d) | ... | 0xEA(s) | ... |
| `fneg.d <ea(s)>, <ea(d)>` | 0xBF | 0xEA(d) | ... | 0xEA(s) | ... |

___

### MULS

Integer Signed Multiply

___

### MULU

Integer Unsigned Multiply

___

### FMUL

Floating Point Multiply

___

### DIVS

Integer Signed Divide

___

### DIVU

Integer Unsigned Divide

___

### FDIV

Floating Point Divide

___

### FMOD

Floating Point Modulus

___

### FABS

Floating Point Absolute

___

### FSQRT

Floating Point Square Root

___

### FACOS

Floating Point Arccosine

___

### FASIN

Floating Point Arcsine

___

### FATAN

Floating Point Arctangent

___

### FCOS

Floating Point Cosine

___

### FSIN

Floating Point Sine

___

### FSINCOS

Floating Point Sine and Cosine

___

### FTAN

Floating Point Tangent

___

### FETOX

Floating Point e to x

___

### FLOGN

Floating Point Natural Logarithm

___

### FLOG2

Floating Point Base-2 Logarithm

___

### FTWOTOX

Floating Point 2 to x

___

### FGETEXP

Get Floating Point Exponent

___

### FGETMAN

Get Floating Point Mantissa

<!--
        MULS_B    = self::OFS_ARITHMETIC + 32, // 0xC0
        MULS_W    = self::OFS_ARITHMETIC + 33,
        MULS_L    = self::OFS_ARITHMETIC + 34,
        MULS_Q    = self::OFS_ARITHMETIC + 35,
        MULU_B    = self::OFS_ARITHMETIC + 36,
        MULU_W    = self::OFS_ARITHMETIC + 37,
        MULU_L    = self::OFS_ARITHMETIC + 38,
        MULU_Q    = self::OFS_ARITHMETIC + 39,
        FMUL_S    = self::OFS_ARITHMETIC + 40,
        FMUL_D    = self::OFS_ARITHMETIC + 41,
        DIVS_B    = self::OFS_ARITHMETIC + 42,
        DIVS_W    = self::OFS_ARITHMETIC + 43,
        DIVS_L    = self::OFS_ARITHMETIC + 44,
        DIVS_Q    = self::OFS_ARITHMETIC + 45,
        DIVU_B    = self::OFS_ARITHMETIC + 46,
        DIVU_W    = self::OFS_ARITHMETIC + 47,

        DIVU_L    = self::OFS_ARITHMETIC + 48, // 0xD0
        DIVU_Q    = self::OFS_ARITHMETIC + 49,
        FDIV_S    = self::OFS_ARITHMETIC + 50,
        FDIV_D    = self::OFS_ARITHMETIC + 51,
        FMOD_S    = self::OFS_ARITHMETIC + 52,
        FMOD_D    = self::OFS_ARITHMETIC + 53,
        FABS_S    = self::OFS_ARITHMETIC + 54,
        FABS_D    = self::OFS_ARITHMETIC + 55,
        FSQRT_S   = self::OFS_ARITHMETIC + 56,
        FRQRT_D   = self::OFS_ARITHMETIC + 57,
        FACOS_S   = self::OFS_ARITHMETIC + 58,
        FACOS_D   = self::OFS_ARITHMETIC + 59,
        FASIN_S   = self::OFS_ARITHMETIC + 60,
        FASIN_D   = self::OFS_ARITHMETIC + 61,
        FATAN_S   = self::OFS_ARITHMETIC + 62,
        FATAN_D   = self::OFS_ARITHMETIC + 63,

        FCOS_S    = self::OFS_ARITHMETIC + 64, 0xE0
        FCOS_D    = self::OFS_ARITHMETIC + 65,
        FSIN_S    = self::OFS_ARITHMETIC + 66,
        FSIN_D    = self::OFS_ARITHMETIC + 67,
        FSINCOS_S = self::OFS_ARITHMETIC + 68,
        FSINCOS_D = self::OFS_ARITHMETIC + 69,
        FTAN_S    = self::OFS_ARITHMETIC + 70,
        FTAN_D    = self::OFS_ARITHMETIC + 71,
        FETOX_S   = self::OFS_ARITHMETIC + 72,
        FETOX_D   = self::OFS_ARITHMETIC + 73,
        FLOGN_S   = self::OFS_ARITHMETIC + 74,
        FLOGN_D   = self::OFS_ARITHMETIC + 75,
        FLOG2_S   = self::OFS_ARITHMETIC + 76,
        FLOG2_D   = self::OFS_ARITHMETIC + 77,
        FTWOTOX_S = self::OFS_ARITHMETIC + 78,
        FTWOTOX_D = self::OFS_ARITHMETIC + 79,

        FGETEXP_S = self::OFS_ARITHMETIC + 80, 0xF0
        FGETEXP_D = self::OFS_ARITHMETIC + 81,
        FGETMAN_S = self::OFS_ARITHMETIC + 82,
        FGETMAN_D = self::OFS_ARITHMETIC + 83
-->
