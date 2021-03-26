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
./masm <path to project file>
```

### Project Files

The key to assembling with _masm_ is to define the project file. This is a JSON document. A minimum example might be:

```
{
    "name": "Example",
    "description": "Optional description here.",
    "output": "bin/example.mc64k",
    "options": {
        "log_code_folds": true
    },
    "defines": {
        "F_PI": "#3.1415926535898"
    }
    "sources": [
        "src/example.s"
    ]
}

```

Note that all paths are relative to the location of the project file itself. The following sections are mandatory:

* _name_
    * Short name of the project.
* _output_
    * Location of the output binary,
* _sources_
    * List of sources to assemble.
    * Sources are assembled in the strict order given.
    * Files declared here must exist when _masm_ is invoked.

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
    * Defines a string substitution.
    * `@define <name> <value>`
    * `@def` accepted as short form.
* undefine
    * Removes a string substitution.
    * `@undefine <name>`
    * `@undef` accepted as short form.
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
    * `@export <label>`
    * As the label to export must be global, the directive does not have to be in the same file as the definition.
    * Only exported labels are visible to code outside the assembled binary.
