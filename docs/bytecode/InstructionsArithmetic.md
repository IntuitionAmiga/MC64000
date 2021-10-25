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

Sign Extend Byte.

        <ea(s)> -> <ea(d)>

General syntax:

        extb.<w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `extb.w <ea(s)>, <ea(d)>` | 0x9C | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `extb.l <ea(s)>, <ea(d)>` | 0x9D | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `extb.q <ea(s)>, <ea(d)>` | 0x9E | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### EXTW

Sign Extend Word.

        <ea(s)> -> <ea(d)>

General syntax:

        extw.<l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `extw.l <ea(s)>, <ea(d)>` | 0x9F | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `extw.q <ea(s)>, <ea(d)>` | 0xA0 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### EXTL

Sign Extend Long.

        <ea(s)> -> <ea(d)>

General syntax:

        extl.q <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `extl.q <ea(s)>, <ea(d)>` | 0xA1 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### ASL

Arithmetic Shift Left.

        <ea(d)> <<@ <ea(s)> -> <ea(d)>

General syntax:

        asl.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `asl.b <ea(s)>, <ea(d)>` | 0xA2 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `asl.w <ea(s)>, <ea(d)>` | 0xA3 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `asl.l <ea(s)>, <ea(d)>` | 0xA4 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `asl.q <ea(s)>, <ea(d)>` | 0xA5 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### ASR

Arithmetic Shift Right.

        <ea(d)> @>> <ea(s)> -> <ea(d)>

General syntax:

        asr.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `asr.b <ea(s)>, <ea(d)>` | 0xA6 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `asr.w <ea(s)>, <ea(d)>` | 0xA7 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `asr.l <ea(s)>, <ea(d)>` | 0xA8 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `asr.q <ea(s)>, <ea(d)>` | 0xA9 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### ADD

Add source to destination.

        <ea(d)> + <ea(s)> -> <ea(d)>

General syntax:

        add.<b|w|l|q> <ea(s)>, <ea(d)>
        fadd.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `add.b <ea(s)>, <ea(d)>` | 0xAA | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `add.w <ea(s)>, <ea(d)>` | 0xAB | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `add.l <ea(s)>, <ea(d)>` | 0xAC | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `add.q <ea(s)>, <ea(d)>` | 0xAD | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fadd.s <ea(s)>, <ea(d)>` | 0xAE | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fadd.d <ea(s)>, <ea(d)>` | 0xAF | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### SUB

Subtract source from destination.

        <ea(d)> - <ea(s)> -> <ea(d)>

General syntax:

        sub.<b|w|l|q> <ea(s)>, <ea(d)>
        fsub.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `sub.b <ea(s)>, <ea(d)>` | 0xB0 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `sub.w <ea(s)>, <ea(d)>` | 0xB1 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `sub.l <ea(s)>, <ea(d)>` | 0xB2 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `sub.q <ea(s)>, <ea(d)>` | 0xB3 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fsub.s <ea(s)>, <ea(d)>` | 0xB4 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fsub.d <ea(s)>, <ea(d)>` | 0xB5 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### NEG

Negate Source to Destination.

         - <ea(s)> -> <ea(d)>

General syntax:

        neg.<b|w|l|q> <ea(s)>, <ea(d)>
        fneg.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `neg.b <ea(s)>, <ea(d)>` | 0xB6 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `neg.w <ea(s)>, <ea(d)>` | 0xB7 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `neg.l <ea(s)>, <ea(d)>` | 0xB8 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `neg.q <ea(s)>, <ea(d)>` | 0xB9 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fneg.s <ea(s)>, <ea(d)>` | 0xBA | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fneg.d <ea(s)>, <ea(d)>` | 0xBB | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### MULS

Integer Signed Multiply.

        <ea(d)> * <ea(s)> -> <ea(d)>

General syntax:

        muls.<b|w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `muls.b <ea(s)>, <ea(d)>` | 0xBC | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `muls.w <ea(s)>, <ea(d)>` | 0xBD | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `muls.l <ea(s)>, <ea(d)>` | 0xBE | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `muls.q <ea(s)>, <ea(d)>` | 0xBF | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### MULU

Integer Unsigned Multiply.

        <ea(d)> * <ea(s)> -> <ea(d)>

General syntax:

        muls.<b|w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `mulu.b <ea(s)>, <ea(d)>` | 0xC0 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `mulu.w <ea(s)>, <ea(d)>` | 0xC1 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `mulu.l <ea(s)>, <ea(d)>` | 0xC2 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `mulu.q <ea(s)>, <ea(d)>` | 0xC3 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FMUL

Floating Point Multiply.

        <ea(d)> * <ea(s)> -> <ea(d)>

General syntax:

        fmul.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmul.s <ea(s)>, <ea(d)>` | 0xC4 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmul.d <ea(s)>, <ea(d)>` | 0xC5 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### DIVS

Integer Signed Divide.

        <ea(d)> / <ea(s)> -> <ea(d)>

General syntax:

        divs.<l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `divs.l <ea(s)>, <ea(d)>` | 0xC6 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `divs.q <ea(s)>, <ea(d)>` | 0xC7 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### MODS

Integer Signed Modulus.

        <ea(d)> % <ea(s)> -> <ea(d)>

General syntax:

        mods.<l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `divs.l <ea(s)>, <ea(d)>` | 0xC8 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `divs.q <ea(s)>, <ea(d)>` | 0xC9 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### DIVU

Integer Unsigned Divide.

        <ea(d)> / <ea(s)> -> <ea(d)>

General syntax:

        divu.<l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `divu.l <ea(s)>, <ea(d)>` | 0xCA | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `divu.q <ea(s)>, <ea(d)>` | 0xCB | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### MODU

Integer Unsigned Modulus.

        <ea(d)> % <ea(s)> -> <ea(d)>

General syntax:

        modu.<l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `modu.l <ea(s)>, <ea(d)>` | 0xCC | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `modu.q <ea(s)>, <ea(d)>` | 0xCD | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FDIV

Floating Point Divide.

        <ea(d)> / <ea(s)> -> <ea(d)>

General syntax:

        fdiv.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fdiv.s <ea(s)>, <ea(d)>` | 0xCE | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fdiv.d <ea(s)>, <ea(d)>` | 0xCF | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FMOD

Floating Point Modulus.

        <ea(d)> % <ea(s)> -> <ea(d)>

General syntax:

        fmod.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmod.s <ea(s)>, <ea(d)>` | 0xD0 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmod.d <ea(s)>, <ea(d)>` | 0xD1 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FABS

Floating Point Absolute.

        abs(<ea(s)>) -> <ea(d)>

General syntax:

        fabs.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fabs.s <ea(s)>, <ea(d)>` | 0xD2 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fabs.d <ea(s)>, <ea(d)>` | 0xD3 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FSQRT

Floating Point Square Root.

        sqrt(<ea(s)>) -> <ea(d)>

General syntax:

        fqrt.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fsqrt.s <ea(s)>, <ea(d)>` | 0xD4 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fsqrt.d <ea(s)>, <ea(d)>` | 0xD5 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FACOS

Floating Point Arccosine.

        acos(<ea(s)>) -> <ea(d)>

General syntax:

        facos.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `facos.s <ea(s)>, <ea(d)>` | 0xD6 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `facos.d <ea(s)>, <ea(d)>` | 0xD7 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FASIN

Floating Point Arcsine.

        asin(<ea(s)>) -> <ea(d)>

General syntax:

        fasin.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fasin.s <ea(s)>, <ea(d)>` | 0xD8 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fasin.d <ea(s)>, <ea(d)>` | 0xD9 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FATAN

Floating Point Arctangent.

        atan(<ea(s)>) -> <ea(d)>

General syntax:

        fatan.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fatan.s <ea(s)>, <ea(d)>` | 0xDA | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fatan.d <ea(s)>, <ea(d)>` | 0xDB | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FCOS

Floating Point Cosine.

        cos(<ea(s)>) -> <ea(d)>

General syntax:

        fcos.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fcos.s <ea(s)>, <ea(d)>` | 0xDC | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fcos.d <ea(s)>, <ea(d)>` | 0xDD | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FSIN

Floating Point Sine.

        sin(<ea(s)>) -> <ea(d)>

General syntax:

        fsin.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fsin.s <ea(s)>, <ea(d)>` | 0xDE | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fsin.d <ea(s)>, <ea(d)>` | 0xDF | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FSINCOS

Floating Point Sine and Cosine.

        sin(<ea(s)>) -> <ea(d)>, cos(<ea(s)>) -> <ea(s)>

General syntax:

        fsin.<s|d> <ea(s)>, <ea(d)>

* The source operand is also used as a destination, therefore cannot be immediate.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fsincos.s <ea(s)>, <ea(d)>` | 0xE0 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fsincos.d <ea(s)>, <ea(d)>` | 0xE1 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FTAN

Floating Point Tangent.

        tan(<ea(s)>) -> <ea(d)>

General syntax:

        ftan.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `ftan.s <ea(s)>, <ea(d)>` | 0xE2 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `ftan.d <ea(s)>, <ea(d)>` | 0xE3 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FETOX

Floating Point e to x.

        e^<ea(s)> -> <ea(d)>

General syntax:

        fetox.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fetox.s <ea(s)>, <ea(d)>` | 0xE4 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fetox.d <ea(s)>, <ea(d)>` | 0xE5 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FLOGN

Floating Point Natural Logarithm.

        ln(<ea(s)>) -> <ea(d)>

General syntax:

        flogn.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `flogn.s <ea(s)>, <ea(d)>` | 0xE6 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `flogn.d <ea(s)>, <ea(d)>` | 0xE7 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FLOG2

Floating Point Base-2 Logarithm.

        log2(<ea(s)>) -> <ea(d)>

General syntax:

        flog2.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `flog2.s <ea(s)>, <ea(d)>` | 0xE8 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `flog2.d <ea(s)>, <ea(d)>` | 0xE9 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___

### FTWOTOX

Floating Point 2 to x

        2.0^<ea(s)> -> <ea(d)>

General syntax:

        ftwotox.<s|d> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `ftwotox.s <ea(s)>, <ea(d)>` | 0xEA | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `ftwotox.d <ea(s)>, <ea(d)>` | 0xEB | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
___
