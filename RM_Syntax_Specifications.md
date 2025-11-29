# RM Compiler – Syntax Specification

This document defines the syntax rules and language constructs supported by the **RM Compiler (Level-4 C Compiler)**.  
It serves as a reference for all valid program structures, data types, operators, and statements recognized by the compiler.

---

## 📘 1. Data Types

The compiler currently supports the following primitive and composite data types:

| Type | Description | Example |
|------|-------------|------|
| `int` | Integer type | `int a = 10;` |
| `double` | Floating-point type | `double x = 3.14;` |
| `bool` | Boolean type | `bool flag = true;` |
| `char` | Character type | `char c = 'A';` |
| `int[]` / `double[]` / `char[]` / `bool[]` | One-dimensional arrays | `int arr[5];`, `char name[10];` |

> **Array Features:**
> - Array size can be an integer literal or a variable name: `int arr[n];`
> - Arrays can be initialized with constant values: `int arr[3] = {1, 2, 3};`
> - Variable-sized arrays cannot be initialized (only declared)

---

## 🧮 2. Literals

| Category | Examples |
|----------|----------|
| Integer literals | `0`, `25`, `42`, `100` |
| Double literals | `3.14`, `0.5`, `2.75`, `10.0` |
| Boolean literals | `true`, `false` |
| Character literals | `'a'`, `'Z'`, `'0'`, `'\n'` |
| String literals | `"hello"`, `"world"` |

> **Note:** Negative numbers are not directly supported as literals but can be represented using subtraction (e.g., `a = 0 - 5;`).

---

## ⚙️ 3. Operators

| Type | Operators Supported | Example |
|------|---------------------|------|
| Arithmetic | `+`, `-`, `*`, `/` | `a + b * c` |
| Relational | `==`, `!=`, `<`, `>`, `<=`, `>=` | `a < b`, `x >= y` |
| Logical | `&&`, `\|\|`, `!` | `(a < b) && (b < c) \|\| !flag` |
| Unary | `++`, `--` (prefix and postfix) | `++i;`, `--j;`, `i++;`, `j--;` |
| Assignment | `=` | `a = b + c;` |

### Operator Precedence (Highest to Lowest)

| Precedence | Operators | Description |
|------------|-----------|-------------|
| 10 | `()` | Parentheses (grouping) |
| 8 | `!`, `++`, `--` | Unary operators |
| 7 | `*`, `/` | Multiplication, Division |
| 6 | `+`, `-` | Addition, Subtraction |
| 5 | `<`, `>`, `<=`, `>=` | Relational operators |
| 4 | `==`, `!=` | Equality operators |
| 3 | `&&` | Logical AND |
| 2 | `\|\|` | Logical OR |
| 1 | `=` | Assignment |
| 0 | `;` | Statement terminator |

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
// Simple declarations
int a;
double x;
bool flag;
char c;

// Declarations with initialization
int a = 10;
double x = 3.14;
bool flag = true;
char c = 'A';

// Array declarations (size can be literal or variable)
int arr[5];
char name[n];          // where n is a previously declared int variable
double values[100];

// Array declarations with initialization (size must be literal)
int example[3] = {1, 2, 3};
char letters[3] = {'a', 'b', 'c'};
double nums[2] = {3.14, 2.71};
bool flags[2] = {true, false};
```

**Rules:**
- Each declaration must specify a type
- Only **one variable per declaration statement**
- Optional initialization at declaration time
- Array size can be an integer literal or a variable name
- Arrays with variable size cannot be initialized (only declared)
- Array initialization requires the size to be a constant integer literal

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
- Must start with a letter (a-z, A-Z)
- Case-sensitive
- Cannot be a reserved keyword
- Cannot contain with a digit

**Valid:**
```c
x, total, count, sum, myArray, Temp
```

**Invalid:**
```c
if          // reserved keyword
while       // reserved keyword  
double      // reserved keyword
1variable   // starts with digit
my-var      // contains hyphen
my var      // contains space
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

ForLoop → for ( Statement ; Expression ; IncrementDecrement ) { StatementList }

Expression → Term ((+|-) Term)*
Term → Factor ((*|/) Factor)*
Factor → ID | INTEGER | DOUBLE | CHAR | BOOL | ( Expression ) | ID [ Expression ]
       | ! Factor | ++ ID | -- ID | ID ++ | ID --

Type → int | char | double | bool
```

---

## 🔍 11. Type Checking and Semantic Rules

### Type Compatibility

**Arithmetic Operators (`+`, `-`, `*`, `/`)**:
- Both operands must be `int` or `double`
- If both are `int`, result is `int`
- If either is `double`, result is `double`

**Relational Operators (`<`, `>`, `<=`, `>=`, `==`, `!=`)**:
- Operands can be `int`, `double`, or `bool`
- Result is always `bool`
- Warning issued if operand types differ

**Logical Operators (`&&`, `||`)**:
- Both operands must be `bool`
- Result is `bool`

**Unary NOT (`!`)**:
- Operand must be `bool`
- Result is `bool`

**Increment/Decrement (`++`, `--`)**:
- Operand must be `int`
- Can be used as prefix or postfix
- Result is `int`

### Conditions in Control Structures

- Conditions in `if`, `else`, `while`, and `for` statements should be of type `bool`
- Warning issued if condition is not boolean type

### Array Rules

- Array index must be of type `int`
- Array size in declaration must be `int` (literal or variable)
- Variable used as array size must be declared and initialized

### Scope and Visibility

- Variables must be declared before use
- Variables in inner blocks can access outer scope variables
- Each control structure (`if`, `else`, `while`, `for`) creates a new scope
- Duplicate variable names in the same scope are not allowed

---

## 📌 Notes

- All statements must end with a semicolon (`;`)
- Blocks are delimited by curly braces (`{}`)
- Comments are supported (both `//` single-line and `/* */` multi-line)
- Only **one variable** can be declared per declaration statement
- Parenthesized expressions on the left side of assignment are **not supported**
- When declaring arrays, the size must be either an integer literal or a variable name (expressions like `arr[5+3]` are not allowed in declarations)
- When accessing arrays, expressions are allowed: `arr[i + 1]` is valid
- Variable-sized arrays cannot be initialized at declaration
- In **for** loops, only **one** update statement is allowed
- All code must be inside the main function's curly braces `{}`
- Negative number literals are not directly supported (use subtraction: `a = 0 - 5;`)

---



**RM Compiler** – A subset C compiler for educational purposes.
