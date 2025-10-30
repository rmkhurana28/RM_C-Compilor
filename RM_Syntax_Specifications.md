# RM Compiler – Syntax Specification

This document defines the syntax rules and language constructs supported by the **RM Compiler (Level-2 C Compiler)**.  
It serves as a reference for all valid program structures, data types, operators, and statements recognized by the compiler.

---

## 📘 1. Data Types

The compiler currently supports the following primitive and composite data types:

| Type | Description | Example |
|------|-------------|---------|
| `int` | Integer type | `int a = 10;` |
| `double` | Floating-point type | `double x = 3.14;` |
| `bool` | Boolean type | `bool flag = true;` |
| `char` | Character type | `char c = 'A';` |
| `int[]` / `double[]` / `char[]` / `bool[]` | One-dimensional arrays | `int arr[5];`, `char name[10];` |

> Array initialization with constant numbers are supported

---

## 🧮 2. Literals

| Category | Examples |
|----------|----------|
| Integer literals | `0`, `25`, `42`, `100` |
| Double literals | `3.14`, `0.5`, `2.75`, `10.0` |
| Boolean literals | `true`, `false` |
| Character literals | `'a'`, `'Z'`, `'0'`, `'\n'` |

> Negative numbers are **not directly supported** as literals but can be represented using subtraction (e.g., `a = 0 - 5;`).

---

## ⚙️ 3. Operators

| Type | Operators Supported | Example |
|------|---------------------|---------|
| Arithmetic | `+`, `-`, `*`, `/` | `a + b * c` |
| Relational | `==`, `!=`, `<`, `>`, `<=`, `>=` | `a < b`, `x >= y` |
| Logical | `&&`, `\|\|`, `!` | `(a < b) && (b < c) \|\| !flag` |
| Unary | `++`, `--` | `++i;`, `--j;` |
| Assignment | `=` | `a = b + c;` |

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
arr[0] = 10;
arr[5 + n] = 10;
value = arr[i + 1];
```

---

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
char name[n];
char example[3] = {'a' , 'b' , 'c'};
double values[100];
```

**Rules:**
- Declaration must specify a type
- Optional initialization at declaration time

---

### ➤ Assignment Statements

```c
a = 10;
x = y + z;
arr[2] = 42;
flag = (a > b);
```

---

### ➤ Increment/Decrement Statements

```c
++i;
--j;
i++;
j--;
```

> Both prefix and postfix forms are supported.

---

### ➤ Conditional Statements (`if`/`else`)

```c
if (condition) {
    // statements
}

if (condition) {
    // statements
} else {
    // statements
}
```

**Example:**
```c
if (a > b) {
    max = a;
} else {
    max = b;
}
```

---

### ➤ Loop Statements

#### **`while` loop**
```c
while (condition) {
    // statements
}
```

**Example:**
```c
int i = 0;
while (i < 10) {
    sum = sum + i;
    ++i;
}
```

#### **`for` loop**
```c
for (initialization; condition; update) {
    // statements
}
```

**Example:**
```c
for (i = 0; i < 10; ++i) {
    arr[i] = i * 2;
}
```

---

## 🔤 6. Identifiers

Identifiers (variable names, array names) must follow these rules:
- Case-sensitive
- Cannot be a reserved keyword

**Valid:**
```c
x, total, _count, sum1, myArray
```

**Invalid:**
```c
if, while, double
```

---

## 🔑 7. Reserved Keywords

The following keywords are reserved and cannot be used as identifiers:

```
int, char, double, bool, if, else, for, while, true, false
```

---

## 📐 8. Punctuation and Delimiters

| Symbol | Purpose |
|--------|---------|
| `;` | Statement terminator |
| `,` | Separator (declarations, parameters) |
| `()` | Grouping, conditions, function calls |
| `[]` | Array indexing |
| `{}` | Block delimiters |

---

## ✅ 9. Example Programs

### **Example 1: Simple Arithmetic**
```c
int a = 10;
int b = 20;
int sum;
sum = a + b;
```

### **Example 2: Conditional Logic**
```c
int x = 5;
int y = 10;
if (x < y) {
    x = x + 1;
} else {
    y = y - 1;
}
```

### **Example 3: Loop with Array**
```c
int arr[5];
int i;
for (i = 0; i < 5; ++i) {
    arr[i] = i * 10;
}
```

### **Example 4: Boolean Logic**
```c
bool flag = true;
int a = 5;
int b = 10;
if (flag && (a < b)) {
    a = a + 1;
}
```

---

## 📝 10. Grammar Summary

```
Program → StatementList

StatementList → Statement StatementList | ε

Statement → Declaration
          | Assignment
          | IfStatement
          | WhileLoop
          | ForLoop
          | IncrementDecrement

Declaration → Type ID ;
            | Type ID = Expression ;
            | Type ID [ INTEGER ] ;

Assignment → ID = Expression ;
           | ID [ Expression ] = Expression ;

IncrementDecrement → ++ ID ;
                   | -- ID ;
                   | ID ++ ;
                   | ID -- ;

IfStatement → if ( Expression ) { StatementList }
            | if ( Expression ) { StatementList } else { StatementList }

WhileLoop → while ( Expression ) { StatementList }

ForLoop → for ( Statement Expression ; IncrementDecrement ) { StatementList }

Expression → Term ((+|-) Term)*
Term → Factor ((*|/) Factor)*
Factor → ID | INTEGER | DOUBLE | CHAR | BOOL | ( Expression ) | ID [ Expression ]
       | ! Factor | ++ ID | -- ID | ID ++ | ID --

Type → int | char | double | bool
```

---

## 📌 Notes

- All statements must end with a semicolon (`;`)
- Blocks are delimited by curly braces (`{}`)
- ( ) are **NOT supported** on left side of assignment
- Comments are **not supported** in the current version
- in **for** loop, only 1 update is allowed at update section

---

**RM Compiler** – A subset C compiler for educational purposes.
