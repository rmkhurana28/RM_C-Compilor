# RM Compiler – Level-2 C Compiler (Built in C)

### 🧠 What This Compiler Supports

- Variable declarations (`int`, `double`, `bool`, `char`, arrays)
- Expressions (arithmetic, logical, relational)
- Assignment statements
- Conditional statements (`if`, `else`)
- Looping constructs (`while`, `for`)
- Array access and initialization
- Abstract Syntax Tree (AST) generation and visualization
- Three-Address Code (TAC) generation
- Control Flow Graph (CFG) and basic block formation
- Code optimization techniques
- Error reporting with descriptive error codes

---

## 📖 Overview

The **RM Compiler** is a fully functional **Level-2 C Compiler** implemented in the **C language**.  

It is **under advanced development**, designed to evolve into a complete and modular compiler capable of handling multiple phases of the compilation process.

It performs all major phases of compilation, including:

- **Lexical Analysis** – Tokenizes source code into meaningful symbols  
- **Syntax Analysis (Parsing)** – Builds a structured parse tree  
- **Semantic Analysis** – Validates types and variable declarations  
- **Intermediate Code Generation** – Produces three-address code (TAC)  
- **Code Optimization** – Optimizes intermediate representations for efficiency  
- **Target Code Generation** – Generates low-level executable instructions  

The compiler is designed with a **modular architecture**, ensuring that each phase (lexer, parser, semantic analyzer, code generator, and optimizer) is cleanly separated for clarity and extensibility.

---

## 🚀 Getting Started

### Prerequisites

- **GCC** or any C compiler
- **Linux/Unix** environment (tested on Fedora/Ubuntu)
- **Make** (optional, for build automation)

### Compilation

To compile the RM Compiler, use the following command:

```bash
gcc -o Main Main.c 00_print.c 01_genTokens.c 02_genAST.c 03_semanticCheck.c 04_icg.c 05_optimization.c 06_tcg.c database.c
```

Or simply:

```bash
make
```

---

## 🎯 Usage

To compile a C source file using the RM Compiler:

```bash
./Main <source_file.c>
```

**Example:**

```bash
./Main tester/test.c
```

This will:
1. Tokenize the input source code
2. Generate and print the Abstract Syntax Tree (AST)
3. Perform semantic analysis and print the symbol table
4. Generate three-address code (TAC)
5. Perform optimization and generate basic blocks with control flow graph
6. Generate target code (if enabled)

---

## 📂 Project Structure

```
02/
├── Main.c                    # Entry point of the compiler
├── database.h                # Shared data structures and declarations
├── database.c                # Utility functions for data structures
├── 00_print.c                # Pretty printing functions for tokens, AST, TAC, blocks
├── 01_genTokens.c            # Lexical analyzer (tokenizer)
├── 02_genAST.c               # Syntax analyzer (parser, AST generator)
├── 03_semanticCheck.c        # Semantic analyzer (type checking, scope validation)
├── 04_icg.c                  # Intermediate code generator (TAC generation)
├── 05_optimization.c         # Optimizer (CFG, basic blocks, optimizations)
├── 06_tcg.c                  # Target code generator
├── 06_01_tcg_real.c          # Advanced target code generator
├── tester/                   # Test directory
│   └── test.c                # Sample input programs
├── README.md                 # This file
└── RM_Syntax_Specification.md # Language syntax specification
```

---

## 🔧 Compilation Phases

### 1. **Lexical Analysis (Tokenization)**
- Reads source code character by character
- Generates tokens (keywords, identifiers, operators, literals, etc.)
- Handles comments and whitespace

### 2. **Syntax Analysis (Parsing)**
- Builds an Abstract Syntax Tree (AST) from tokens
- Validates syntactic correctness based on grammar rules
- Supports nested expressions, control structures, and declarations

### 3. **Semantic Analysis**
- Type checking and type inference
- Variable declaration and scope validation
- Symbol table management
- Reports semantic errors (undeclared variables, type mismatches, etc.)

### 4. **Intermediate Code Generation (ICG)**
- Generates three-address code (TAC) from the AST
- Converts high-level constructs into simple instructions
- Produces linearized intermediate representation

### 5. **Code Optimization**
- Constructs Control Flow Graph (CFG)
- Forms basic blocks from three-address code
- Performs data flow analysis (reaching definitions)
- Applies various optimization techniques to improve code efficiency

### 6. **Target Code Generation**
- Generates x86-64 assembly code
- Implements register allocation and instruction selection
- Produces fully assemblable output
- Supports compilation to executable binaries

---

## 📊 Example

### Input (`test.c`):

```c
int main() {
    int a;
    int b;
    int c;
    
    a = 5;
    b = 10;
    
    if (a > b) {
        c = a + b;
    } else {
        c = b - a;
    }
    
    return 0;
}
```

### Output:

The compiler will produce:
1. **Token list** – All lexical tokens identified
2. **AST** – Hierarchical tree representation of the program
3. **Symbol Table** – All declared variables with their types and scopes
4. **Three-Address Code** – Intermediate representation with temporary variables
5. **Basic Blocks & CFG** – Control flow graph showing program structure
6. **Optimized Code** – Improved intermediate representation
7. **x86-64 Assembly** – Generated assembly code saved to external file

---

## 🐛 Error Handling

The compiler provides detailed error messages for:
- **Lexical errors** – Invalid characters or malformed tokens
- **Syntax errors** – Grammar violations
- **Semantic errors** – Type mismatches, undeclared variables, scope violations

Each error includes:
- Error code (e.g., `E || 03.01`)
- Descriptive message
- Context information

---

## 📝 Language Specification

### Supported Data Types:
- `int` – Integer values
- `double` – Floating-point values
- `bool` – Boolean values (true/false)
- `char` – Character values
- Arrays of all above types

### Supported Operators:
- **Arithmetic:** `+`, `-`, `*`, `/`, `%`
- **Relational:** `>`, `<`, `>=`, `<=`, `==`, `!=`
- **Logical:** `&&`, `||`, `!`
- **Assignment:** `=`

### Supported Statements:
- Variable declarations and assignments
- `if`, `else` conditionals
- `while`, `for` loops
- Array initialization and access

---

## 🛣️ Roadmap

### ✅ Completed:
- Lexical analysis
- Syntax analysis with AST generation
- Semantic analysis with symbol table
- Three-address code generation
- Control flow graph and basic block formation
- Optimization framework
- Target code generation (x86-64 assembly)

### 🔄 In Progress:
- Advanced optimization techniques

---

## 👨‍💻 Author

**Ridham Khurana (RM)**  
Level-2 C Compiler Implementation

---

## 📄 License

This project is an educational compiler implementation.  
Feel free to use and modify for learning purposes.

---

## � Acknowledgments

Built as part of compiler design coursework, incorporating principles from:
- "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- Modern compiler construction practices
- Data flow analysis and optimization theory

---

## 💬 Contact

For questions, suggestions, or bug reports, please open an issue on the repository.

