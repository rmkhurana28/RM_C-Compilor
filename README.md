# RMc4 Compiler – A Complete C Compiler Implementation

<div align="center">

![Compiler](https://img.shields.io/badge/Language-C-blue.svg)   
![License](https://img.shields.io/badge/License-Educational-green.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-orange.svg)

**A full-featured, multi-phase compiler built from scratch in C**

**RMc4** = **RM** (Ridham Khurana) + **c4** (Level-4 Compiler)

</div>

---

## 📖 Overview

The **RMc4 Compiler** is a complete Level-4 compiler implementation for a C-like language, built entirely in C. The name **RMc4** represents **RM** (the author, Ridham Khurana) and **c4** (signifying a Level-4 C Compiler). This project demonstrates all major phases of compilation, from lexical analysis to x86-64 code generation, with particular emphasis on code optimization techniques.

### 🎯 Key Features

- ✅ **Complete Compilation Pipeline** – All phases from tokenization to target code generation
- 🔍 **Lexical Analysis** – Robust tokenizer with keyword, operator, and literal recognition
- 🌳 **Syntax Analysis** – Recursive descent parser generating Abstract Syntax Trees
- 🔬 **Semantic Analysis** – Type checking, scope validation, and symbol table management
- 📊 **Intermediate Code Generation** – Three-address code (TAC) generation
- ⚡ **Code Optimization** – CFG construction, basic blocks, data flow analysis, constant folding, dead code elimination
- 🎯 **Target Code Generation** – Real x86-64 assembly output
- 📝 **Comprehensive Error Reporting** – Detailed error codes with context information
- 🧪 **Extensive Test Suite** – 40+ valid test cases, 25+ invalid test cases

---

## 🧠 Supported Language Features

### Data Types
- `int` – Integer values
- `double` – Floating-point values
- `bool` – Boolean values (true/false)
- `char` – Character values
- Arrays of all types (with initialization support)

### Operators
- **Arithmetic:** `+`, `-`, `*`, `/`
- **Relational:** `<`, `>`, `<=`, `>=`, `==`, `!=`
- **Logical:** `&&`, `||`, `!`
- **Unary:** `++`, `--` (both prefix and postfix)
- **Assignment:** `=`

### Control Structures
- `if` / `else` statements
- `while` loops
- `for` loops
- Nested control structures

### Other Features
- Variable declarations with optional initialization
- Array declarations and initializations
- Array element access and modification
- Expression evaluation with correct operator precedence
- Compound statements (blocks)

---

## 🚀 Getting Started

### Prerequisites

- **GCC** or any C compiler
- **GNU Make** (optional, for automated builds)
- **Linux/Unix** environment (tested on Fedora/Ubuntu)

### Compilation

#### Using Makefile (Recommended)

The project includes a Makefile for easy compilation:

```bash
make
```

This will:
- Compile all source files with warning flags (`-Wall -Wextra`)
- Link the math library (`-lm`)
- Produce the `RMc4` executable

Other Makefile targets:
```bash
make clean    # Remove compiled files and outputs
make rebuild  # Clean and rebuild the entire project
```

#### Manual Compilation

If you prefer to compile manually without Make:

```bash
gcc -o RMc4 Main.c 00_print.c 00_01_printToFile.c 01_genTokens.c 02_genAST.c \
    03_semanticCheck.c 04_icg.c 05_optimization.c 06_tcg.c 06_01_tcg_real.c \
    database.c -lm -Wall -Wextra
```

### Running the Compiler

```bash
./RMc4 <source_file.c> compiler_output.txt
```

**Example:**
```bash
./RMc4 tester/test.c compiler_output.txt
```

**Note:** The output filename **must** be `compiler_output.txt` (this is enforced by the compiler).

The compiler will process the input file through all compilation phases and generate:
- **Terminal Output:** Phase progress and timing information
- **File Output:** Detailed compilation results in `compiler_output.txt`

---

## 📂 Project Structure

```
.
├── Main.c                      # Entry point and compilation orchestration
├── database.h                  # Core data structures and type definitions
├── database.c                  # Utility functions
│
├── 00_print.c                  # Console output functions (disabled)
├── 00_01_printToFile.c         # File output functions (active)
│
├── 01_genTokens.c              # Lexical analyzer (tokenizer)
├── 02_genAST.c                 # Syntax analyzer (AST generator)
├── 03_semanticCheck.c          # Semantic analyzer (type checker)
├── 04_icg.c                    # Intermediate code generator (TAC)
├── 05_optimization.c           # Code optimizer (CFG, basic blocks, optimizations)
├── 06_tcg.c                    # Basic target code generator
├── 06_01_tcg_real.c            # Real x86-64 assembly generator
│
├── Makefile                    # Build automation
├── README.md                   # This file
├── RM_Syntax_Specifications.md # Language syntax specification
├── err.md                      # Error code documentation
│
├── output/                     # Compilation output directory
│   └── compiler_output.txt     # Detailed compilation results
│
└── tester/                     # Test suite
    ├── test.c                  # Main test file
    ├── valid_test_cases/       # Test cases that should compile successfully
    └── invalid_test_cases/     # Test cases that should fail with errors
```

---

## 🔧 Compilation Phases

### Phase 1: Lexical Analysis (Tokenization)
**File:** `01_genTokens.c`

- Reads source code character by character
- Recognizes keywords, identifiers, operators, literals, and delimiters
- Generates a stream of tokens
- Handles comments and whitespace
- Produces error messages for invalid tokens

**Output:** Token stream with types and values

---

### Phase 2: Syntax Analysis (Parsing)
**File:** `02_genAST.c`

- Builds Abstract Syntax Tree (AST) from token stream
- Implements recursive descent parsing
- Handles operator precedence and associativity
- Supports nested expressions and compound statements
- Validates syntactic correctness

**Output:** Hierarchical AST representation

---

### Phase 3: Semantic Analysis
**File:** `03_semanticCheck.c`

- Type checking for all expressions and operations
- Variable declaration and scope validation
- Symbol table construction and management
- Array bounds checking
- Operator compatibility verification
- Generates warnings for unused variables

**Output:** Symbol table and semantic validation results

---

### Phase 4: Intermediate Code Generation (ICG)
**File:** `04_icg.c`

- Converts AST to three-address code (TAC)
- Generates temporary variables
- Creates labels for control flow
- Linearizes complex expressions
- Simplifies subsequent optimization and code generation

**Output:** Three-address code instruction stream

---

### Phase 5: Code Optimization
**File:** `05_optimization.c`

Implements multiple optimization techniques:

- **Control Flow Graph (CFG) Construction**
  - Identifies basic blocks
  - Builds flow relationships between blocks
  - Visualizes program control flow

- **Data Flow Analysis**
  - Reaching definitions
  - Live variable analysis
  - Use-def chains

- **Optimizations Applied**
  - Constant folding
  - Constant propagation
  - Copy propagation
  - Dead code elimination (DCE)
  - Common subexpression elimination

**Output:** Optimized TAC and basic block structure

---

### Phase 6: Target Code Generation
**Files:** `06_tcg.c`, `06_01_tcg_real.c`

- Generates x86-64 assembly code
- Register allocation and management
- Stack frame setup
- Variable-to-stack-offset mapping
- Function prologue and epilogue generation
- Instruction selection

**Output:** x86-64 assembly code (ready to assemble)

---

## 📊 Example

### Input Program (`tester/test.c`)

```c
int main() {
    int a;
    int b;
    int result;
    
    a = 10;
    b = 20;
    
    if (a < b) {
        result = b - a;
    } else {
        result = a - b;
    }
    
    return 0;
}
```

### Terminal Output

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Compiling Program: tester/test.c
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

⚙️  Generating tokens...
✓ Token generation completed [took 1.234 ms]

⚙️  Generating AST...
✓ AST generation completed [took 2.345 ms]

⚙️  Performing semantic check...
✓ Semantic check completed [took 0.987 ms]

⚙️  Generating 3-address code...
✓ 3-address code generation completed [took 1.456 ms]

⚙️  Running optimization process...
✓ Optimization process completed [took 3.789 ms]

⚙️  Generating target code (x86-64)...
✓ Target code generation completed [took 2.012 ms]

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Total compilation time: 11.823 ms
  Output saved to: compiler_output.txt
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
✅ Compilation completed successfully!
```

### File Output (`compiler_output.txt`)

Contains detailed information:
- Complete token list with types and values
- ASCII visualization of the AST
- Symbol table with all variables and their properties
- Three-address code before and after optimization
- CFG (Control Flow Graph) structure
- Optimization statistics (instruction counts, reduction percentage)
- Generated x86-64 assembly code
- Basic blocks with leader instructions
- Control flow graph representation
- Final x86-64 assembly code

---

## 🐛 Error Handling

The compiler provides comprehensive error reporting with:

- **Error Codes:** Structured error identification (e.g., `[03.05]`)
- **Phase Identification:** Knows which phase detected the error
- **Descriptive Messages:** Clear explanation of what went wrong
- **Context Information:** Line numbers and relevant details

### Error Categories

- **01.xx** – Lexical errors (invalid tokens, unterminated strings)
- **02.xx** – Syntax errors (grammar violations, missing tokens)
- **03.xx** – Semantic errors (type mismatches, undeclared variables)
- **04.xx** – ICG errors (code generation failures)
- **05.xx** – Optimization errors (analysis failures)
- **06.xx** – Target code generation errors

See `err.md` for complete error code documentation.

---

## 🧪 Testing

### Test Suite Organization

```
tester/
├── test.c                    # Primary test file
├── valid_test_cases/         # Should compile successfully
│   ├── test_01_basic_features.c
│   ├── test_02_arrays_loops.c
│   ├── test_03_complex_control.c
│   └── ... (more tests)
└── invalid_test_cases/       # Should fail with specific errors
    ├── invalid_01_undeclared_var.c
    ├── invalid_02_type_mismatch.c
    ├── invalid_03_double_declaration.c
    └── ... (more tests)
```

### Running Tests

Test individual files:
```bash
./RMc4 tester/valid_test_cases/test_01_basic_features.c compiler_output.txt
./RMc4 tester/invalid_test_cases/invalid_01_undeclared_var.c compiler_output.txt
```

Run all tests:
```bash
./run_all_tests.sh
```

---

## 🛠️ Technical Details

### Memory Management
- Static allocation for most data structures
- Controlled dynamic allocation for specific components
- Bounded arrays with `MAX` constants for safety

### Algorithm Highlights
- **Recursive descent parsing** for syntax analysis
- **Symbol table** implemented as array-based structure with scope tracking
- **Operator precedence** handled via lookup table
- **CFG construction** using leader-based basic block identification
- **Data flow analysis** using iterative algorithms

### Performance
- Typical compilation time: < 20ms for small programs
- Optimized for educational clarity over raw performance
- Scales well for programs up to several hundred lines

---

## 📚 Documentation

- **[RM_Syntax_Specifications.md](RM_Syntax_Specifications.md)** – Complete language syntax reference
- **[err.md](err.md)** – Comprehensive error code documentation
- **Code Comments** – All source files include detailed inline documentation

---

## 🎓 Educational Value

This compiler serves as an excellent learning resource for:
- Understanding compiler design principles
- Learning multi-phase compilation
- Studying data structure applications
- Exploring optimization techniques
- Understanding x86-64 architecture

### Based on Compiler Theory From:
- "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- Modern compiler construction practices
- Data flow analysis theory
- Assembly language and computer architecture

---

## 👨‍💻 Author

**Ridham Khurana (RM)**

This compiler was developed as an educational project to demonstrate deep understanding of compiler design, implementation, and optimization techniques.

---

## 📄 License

This project is released for **educational purposes**. Feel free to use, modify, and learn from the code.

---

## 📚 Acknowledgments

- Compiler design course materials and textbooks
- Open-source compiler projects for inspiration
- x86-64 assembly documentation and references

---

## 📞 Contact

For questions, suggestions, or feedback:
- Open an issue on the repository
- Review the documentation files for detailed information
- Check `err.md` for error-specific queries

---

<div align="center">

**Built with dedication to compiler design excellence**

⭐ If you find this project helpful, please consider starring it!

</div>

