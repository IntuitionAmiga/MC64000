# MASM

## Overview

MASM is the name for the MC64000 bytecode assembler. This commandline tool reads a source project and assembles it into a bytecode binary for execution on an MC64000 runtime. As with the entire project, the tool is very much work in progress.

### Requirements

MASM is written in PHP and requires, as a minimum version 7.4 to run. The first and most obvious question is, why PHP?

* PHP was chosen because it's quick to develop and make changes in.
* PHP has exremely powerful text processing capabilities.
* PHP is adequately fast in execution.

Other than PHP7.4, the only _requirements_ are that JSON support is enabled. This is because configuration and project files are written as JSON documents. The second most obvious question is, why JSON?

* The best, and some may say _only_, genuinely good and useful thing JavaScript has unleashed on the world.

### Location

The entire assembler sourcecode is located under the `./assembler` in the source tree. There should be a PHP script called _masm_ that has the executable bit set.


## Usage

At the time of writing, usage of _masm_ amounts to:
```
./masm <path to project file> [library path]
```
The libary path tells masm where common shared code libraries exist. The default location is ./libs.

### Project Files

The key to assembling with _masm_ is to define the project file. This is a JSON document. A minimalist example:

```json
{
    "target": {
        "name": "Minimal",
        "version": "1.0.0",
        "description": "Bare bones",
        "output": "bin/minimal.64x",
        "host": {
            "name": "Standard Test Host",
            "version": "1.0.0"
        }
    },
    "options": {
        "log_code_folds": false,
        "log_label_add": false,
        "log_label_ref": false,
        "log_label_resolve": false
    },
    "sources": [
        "src/main.s"
    ],
    "defines": {
    }
}
```

Note that all paths are relative to the location of the project file itself. The following sections are mandatory:

* _target_
    * Defines the name, version and expectd host attributes of the binary.
    * Semantic versioning is used by the host to see if it meets the requirements specified in this section when loading,
* _sources_
    * List of sources to assemble.
    * Sources are assembled in the strict order given.
    * Files declared here must exist when _masm_ is invoked.
    * Paths beginning with `lib:` are accepted. These refer to shared source in the library path.

The optional _options_ section is a key-value set of options for the assembler. All values are scalar. Non-scalar values will be ignored. Currently, the following are defined:

* log_code_folds
    * bool (default _false_)
    * if _true_, code fold optimisation events will be logged.
* log_label_add
    * bool (default _false_)
    * if _true_, label definition events will be logged.
* log_label_ref
    * bool (default _false_)
    * if _true_, label reference events will be logged.
* log_label_resolve
    * bool (default _false_)
    * if _true_, label resolution events will be logged.

Note that options can be locally using _directives_.

The optional _defines_ section is a key-value set of name/string substitutions to declare. These definitions are global but can be removed and redefined locally using _directives_.

### Directives

Directives are instructions for the assembler that are embedded within a source file. Directives are indicated using the `@` sigil and take effect from their point of invocation onwards:

```
    @define ONE #1
    @define BASE_DISPLACEMENT 8
    @enable log_code_folds

    divu.q ONE, BASE_DISPLACEMENT(r15) ; Expect the log to indicate this was folded away
```

The following directives exist:

* define
    * Defines a local source string substitution, file scope only.
    * `@define <name> <value>`
    * `@def` accepted as short form.
* undefine
    * Removes a string substitution.
    * `@undefine <name>`
    * `@undef` accepted as short form.
* equ
    * Similar to define but has global scope. Can be undefined, with current file-scope effect only.
    * `@equ <name> <value>`
* enable
    * Turns on a switch _option_.
    * `@enable <option>`
    * `@en` accepted as short form.
* disable
    * Turns on a switch _option_.
    * `@disable <option>`
    * `@dis` accepted as short form.
* export
    * Indicates that a global label is to be exported.
    * `@export <label> [access flags]`
    * As the label to export must be global, the directive does not have to be in the same file as the definition.
    * Only exported labels are visible to code outside the assembled binary.
    * Access flags are any comination of rwx:
        * r indicates that the label is readable.
        * w indicates that the label is writeable.
        * x indicates that the label is executable.
* import
    * Declares the name of a global label that is to be imported.
    * `@import <label> [access flags]`
    * Imported labels refer to entities that are expected to be exposed to the binary by the host environment.
    * Access flags are the same as for export.
* align
    * Aligns whatever follows the directive to a chosen boundary by padding.
    * `@align <pad byte>, <alignment>`
  
