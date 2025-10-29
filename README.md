# RM Compiler – Level-2 C Compiler (Built in C)

### 🧠 What This Compiler Supports

- Variable declarations (`int`, `double`, `bool` , `char` , arrays)
- Expressions (arithmetic, logical, relational)
- Assignment statements
- Conditional statements (`if`, `else`)
- Looping constructs (`while`, `for`)
- Array access and initialization
- Abstract Syntax Tree (AST) generation and visualization
- Error reporting with descriptive error codes

---

The **RM Compiler** is a fully functional **Level-2 C Compiler** implemented in the **C language**.  

It is **under advanced development**, designed to evolve into a complete and modular compiler capable of handling multiple phases of the compilation process.

It performs all major phases of compilation, including:

- **Lexical Analysis** – Tokenizes source code into meaningful symbols  
- **Syntax Analysis (Parsing)** – Builds a structured parse tree  
- **Semantic Analysis** – Validates types and variable declarations
- **Intermediate Code Generation** – Produces optimized intermediate representations  
- **Code Optimization** – Performs constant folding and basic optimizations  
- **Target Code Generation** – Generates low-level executable instructions or pseudo assembly  

The compiler is designed with a **modular architecture**, ensuring that each phase (lexer, parser, semantic analyzer, and code generator) is cleanly separated for clarity and extensibility.
