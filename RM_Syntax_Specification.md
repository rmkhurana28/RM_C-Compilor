# RM Compiler – Syntax Specification

This document defines the syntax rules and language constructs supported by the **RM Compiler (Level-2 C Compiler)**.  
It serves as a reference for all valid program structures, data types, operators, and statements recognized by the compiler.

---

## 📘 1. Data Types

The compiler currently supports the following primitive and composite data types:

| Type | Description | Example |
|------|--------------|----------|
| `int` | Integer type | `int a = 10;` |
| `double` | Floating-point type | `double x = 3.14;` |
| `bool` | Boolean type | `bool flag = true;` |
| `char` | Character type | `char c = 'A';` |
| `int[]` / `double[]` / `char[]` | One-dimensional arrays | `int arr[5];`, `char name[10];` |

> Array initialization with constant expressions is supported.

---

## 🧮 2. Literals

| Category | Examples |
|-----------|-----------|
| Integer literals | `0`, `25`, `42` |
| Double literals | `3.14`, `0.5`, `2.75` |
| Boolean literals | `true`, `false` |
| Character literals | `'a'`, `'Z'`, `'0'` |

> Negative numbers are **not directly supported** as literals but can be represented using subtraction (e.g., `a = 0 - 5;`).

---

## ⚙️ 3. Operators
| Type | Operators Supported | Example |
|------|----------------------|----------|
| Arithmetic | `+`, `-`, `*`, `/` | `a + b * c` |
| Relational | `==`, `!=`, `<`, `>`, `<=`, `>=` | `a < b` |
| Logical | `&&`, `\|\|`, `!` | `(a < b) && (b < c) \|\| !flag` |
| Unary | `++`, `--` | `++i;`, `--j;` |
| Assignment | `=` | `a = b + c;` |





> The modulus (`%`) operator is **not supported**.

---

## 🧩 4. Expressions

Expressions can include:
- Variables, literals, and supported operators  
- Nested sub-expressions inside parentheses  
- Array element access  

**Examples:**
```c
a = b + 2;
x = (y * 3) - (z / 2);
flag = (a < b) && (b != c);
++i;
--j;
ch = 'A';
```

## 🧱 5. Statements

### ➤ Variable Declarations
Variables must be declared before use.  
Supported forms include:
```c
int a;
double x;
bool flag = true;
char c = 'A';
int arr[5];
char name[10];
```

