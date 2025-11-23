# Error Code Documentation

This document contains all error codes and warning codes used throughout the RM Compiler.

---

## Quick Navigation
- **[Category-wise Errors](#category-wise-errors)** - Browse errors organized by compiler phase and error type
- **[Number-wise Errors](#number-wise-errors)** - Quick lookup by error code number

---

## Table of Contents
- [Category-wise Errors](#category-wise-errors)
  - [File 01: Lexical Analysis (Tokenization)](#file-01-lexical-analysis-tokenization)
  - [File 02: Syntax Analysis (AST Generation)](#file-02-syntax-analysis-ast-generation)
  - [File 03: Semantic Analysis](#file-03-semantic-analysis)
  - [File 04: Intermediate Code Generation (ICG)](#file-04-intermediate-code-generation-icg)
  - [File 05: Optimization](#file-05-optimization)
  - [File 06: Target Code Generation](#file-06-target-code-generation)
- [Number-wise Errors](#number-wise-errors)

---

## Category-wise Errors

### File 01: Lexical Analysis (Tokenization)

| Error Code | Description |
|------------|-------------|
| `[01.01]` | File [filename] NOT found |
| `[01.02]` | Typo in lines above main function |
| `[01.03]` | Token buffer is full (MAX limit reached) |
| `[01.04]` | Unterminated character literal |
| `[01.05]` | Unterminated string literal |

---

### File 02: Syntax Analysis (AST Generation)

### Declaration and Assignment Errors

| Error Code | Description |
|------------|-------------|
| `[02.01]` | Invalid token in the starting of line |
| `[02.02]` | Expected variable name for declaration |
| `[02.03]` | Right side node for assignment of variable NOT generated properly |
| `[02.04]` | Declaration node NOT generated properly for variable |
| `[02.11]` | Expected either =(for initialization) or ;(for declaration) but found some other token |
| `[02.12]` | Expected either =(for initialization) or [(for array) or ;(for declaration) |

### Array-Related Errors

| Error Code | Description |
|------------|-------------|
| `[02.05]` | Size of array needs to be either int value or variable |
| `[02.06]` | Expected ] for the closing of size of array |
| `[02.07]` | Array of variable size can NOT be initialized |
| `[02.08]` | Expected { for starting of initialization of array |
| `[02.09]` | Declaration node NOT generated properly for array initialization |
| `[02.10]` | Declaration node NOT generated properly for array declaration |
| `[02.13]` | Index of array must be either int or variable |
| `[02.14]` | Expected ] for the closing of index of array |
| `[02.25]` | Expected } for closing of initialization of array |
| `[02.27]` | Expected ] for the closing of index of array |
| `[02.28]` | Expected = for the assignment of array |
| `[02.29]` | Expected =(for variable assignment) or [(for array assignment) |

### Expression Parsing Errors

| Error Code | Description |
|------------|-------------|
| `[02.15]` | Expected ) for the closing of ( |
| `[02.16]` | Expected lvalue after unary operator |
| `[02.17]` | Unexpected token found in expression |
| `[02.18]` | Expected ) but found ; |
| `[02.19]` | Expected ] but found ; |
| `[02.20]` | Unexpected ) found in expression |
| `[02.21]` | Unexpected ] found in expression |
| `[02.22]` | Unexpected } found in expression |
| `[02.23]` | Unexpected , found in expression |
| `[02.24]` | Expected unary or binary operator in expression |
| `[02.26]` | Expected ; for the end of statement |
| `[02.38]` | Expected lvalue after unary operator |

### Control Flow Errors

#### IF Statement Errors
| Error Code | Description |
|------------|-------------|
| `[02.30]` | Expected ( for the condition of IF (opening) |
| `[02.31]` | Expected ) for the condition of IF (closing) |
| `[02.32]` | Expected { for the opening of IF block |
| `[02.33]` | Expected if or { for valid if-else block |

#### WHILE Loop Errors
| Error Code | Description |
|------------|-------------|
| `[02.39]` | Expected ( for the condition of WHILE (opening) |
| `[02.40]` | Expected ) for the condition of WHILE (closing) |
| `[02.41]` | Expected { for the opening of WHILE block |

#### FOR Loop Errors
| Error Code | Description |
|------------|-------------|
| `[02.42]` | Expected ( for the starting of FOR init |
| `[02.43]` | Expected ; after init |
| `[02.44]` | Expected ; after cond |
| `[02.45]` | Expected ) for the closing of FOR update |
| `[02.46]` | Expected { for starting of for body |

### Unary Operator Errors

| Error Code | Description |
|------------|-------------|
| `[02.34]` | ! NOT allowed in post-fix [var] |
| `[02.35]` | Expected ; at end of statement [var] |
| `[02.36]` | ! NOT allowed in post-fix [array] |
| `[02.37]` | Expected ; at end of statement [array] |

---

### File 03: Semantic Analysis

### General Semantic Errors

| Error Code | Description |
|------------|-------------|
| `[03.01]` | AST is NOT accessible |
| `[03.02]` | Right expr type doesn't match with declared type |
| `[03.03]` | Right expr type doesn't match with left var type |
| `[03.10]` | AST is NOT accessible |
| `[03.21]` | Unknown AST found |
| `[03.26]` | AST is NOT accessible |

### Type Checking Errors

#### Conditional Statement Type Warnings
| Warning Code | Description |
|--------------|-------------|
| `[W \|\| 03.04]` | Expected BOOL in condition of IF but found something else |
| `[W \|\| 03.05]` | Expected BOOL in condition of IF-ELSE but found something else |
| `[W \|\| 03.06]` | Expected BOOL in condition of WHILE but found something else |
| `[W \|\| 03.07]` | Expected BOOL in condition of FOR but found something else |

#### Operator Type Errors
| Error Code | Description |
|------------|-------------|
| `[03.08]` | Expected BOOL after ! |
| `[03.09]` | Expected INT around ++/-- |
| `[03.12]` | Expected INT/DOUBLE on both sides of binary operator (+ - * /) |
| `[03.13]` | Expected INT/DOUBLE/BOOLEAN on both sides of binary operator (== != > < >= <=) |
| `[03.15]` | Expected BOOLEAN around binop (&& \|\|) |
| `[03.17]` | Expected BOOLEAN after ! |
| `[03.18]` | Expected INT around ++/-- |

#### Binary Operator Errors
| Error/Warning Code | Description |
|--------------------|-------------|
| `[W \|\| 03.14]` | Different data type found around binop |
| `[03.16]` | Unexpected binop code returned |

#### Unary Operator Errors
| Error Code | Description |
|------------|-------------|
| `[03.19]` | Unexpected unary code returned |

### Array and Variable Errors

| Error/Warning Code | Description |
|--------------------|-------------|
| `[03.11]` | Index of array MUST be int |
| `[03.22]` | Variable () already exists in current scope |
| `[03.23]` | Index symbol (%s) is undefined |
| `[03.24]` | Index symbol (%s) MUST be of type INT |
| `[W \|\| 03.25]` | Index Symbol (%s) is NOT initialized |

### FOR Loop Specific Errors

| Error/Warning Code | Description |
|--------------------|-------------|
| `[W \|\| 03.20]` | Unexpected = found in expression |
| `[03.27]` | ! NOT allowed in update section of FOR |
| `[03.28]` | Expected either INCREMENTS/DECREMENTS/ASSIGNMENTS statements in update section of for |

---

### File 04: Intermediate Code Generation (ICG)

### General ICG Errors

| Error Code | Description |
|------------|-------------|
| `[04.01]` | AST is NOT accessible or NULL |
| `[04.06]` | Unknown or unsupported AST node type found |
| `[04.07]` | Invalid operator type in AST node |

### Memory Allocation Errors

| Error Code | Description |
|------------|-------------|
| `[04.02]` | Memory allocation failed for temporary variable |
| `[04.03]` | Memory allocation failed for label |
| `[04.04]` | Memory allocation failed for three-address code instruction |

### Resource Limit Errors

| Error Code | Description |
|------------|-------------|
| `[04.05]` | Maximum limit of three-address code instructions reached |

---

### File 05: Optimization

### Memory Allocation Errors

| Error Code | Description |
|------------|-------------|
| `[05.01]` | Memory allocation failed for basic block |
| `[05.03]` | Memory allocation failed for block properties |
| `[05.05]` | Memory allocation failed for address pointer array |

### Resource Limit and Structure Errors

| Error Code | Description |
|------------|-------------|
| `[05.02]` | Maximum limit of basic blocks reached |
| `[05.04]` | Block index mismatch during property assignment |
| `[05.06]` | Block not found in block array |
| `[05.07]` | Invalid block structure detected |

### Control Flow Graph Errors

| Error Code | Description |
|------------|-------------|
| `[05.08]` | CFG formation error - label not found |

---

### File 06: Target Code Generation (Three-Address Code to Assembly)

### Memory and Resource Errors

| Error Code | Description |
|------------|-------------|
| `[06.01]` | Memory allocation failed for assembly instruction |
| `[06.02]` | Assembly buffer is full - maximum instruction limit reached |

### Code Generation Errors

| Error Code | Description |
|------------|-------------|
| `[06.03]` | Invalid operand type in three-address code |
| `[06.04]` | Unknown instruction type during code generation |
| `[06.05]` | Register allocation failed |
| `[06.06]` | Invalid operation for target code generation |

---

### File 06_01: Real Target Code Generation (x86-64 Assembly)

### Memory and Resource Errors

| Error Code | Description |
|------------|-------------|
| `[06_01.01]` | Real assembly buffer is full - maximum instruction limit reached |
| `[06_01.02]` | Memory allocation failed for real assembly instruction |
| `[06_01.03]` | Variable stack map is full - maximum variable limit reached |

---

## Number-wise Errors

All errors and warnings listed in ascending numeric order for quick lookup.

### File 01 Errors (01.01 - 01.05)

| Error Code | Description |
|------------|-------------|
| `[01.01]` | File [filename] NOT found |
| `[01.02]` | Typo in lines above main function |
| `[01.03]` | Token buffer is full (MAX limit reached) |
| `[01.04]` | Unterminated character literal |
| `[01.05]` | Unterminated string literal |

### File 02 Errors (02.01 - 02.46)

| Error Code | Description |
|------------|-------------|
| `[02.01]` | Invalid token in the starting of line |
| `[02.02]` | Expected variable name for declaration |
| `[02.03]` | Right side node for assignment of variable NOT generated properly |
| `[02.04]` | Declaration node NOT generated properly for variable |
| `[02.05]` | Size of array needs to be either int value or variable |
| `[02.06]` | Expected ] for the closing of size of array |
| `[02.07]` | Array of variable size can NOT be initialized |
| `[02.08]` | Expected { for starting of initialization of array |
| `[02.09]` | Declaration node NOT generated properly for array initialization |
| `[02.10]` | Declaration node NOT generated properly for array declaration |
| `[02.11]` | Expected either =(for initialization) or ;(for declaration) but found some other token |
| `[02.12]` | Expected either =(for initialization) or [(for array) or ;(for declaration) |
| `[02.13]` | Index of array must be either int or variable |
| `[02.14]` | Expected ] for the closing of index of array |
| `[02.15]` | Expected ) for the closing of ( |
| `[02.16]` | Expected lvalue after unary operator |
| `[02.17]` | Unexpected token found in expression |
| `[02.18]` | Expected ) but found ; |
| `[02.19]` | Expected ] but found ; |
| `[02.20]` | Unexpected ) found in expression |
| `[02.21]` | Unexpected ] found in expression |
| `[02.22]` | Unexpected } found in expression |
| `[02.23]` | Unexpected , found in expression |
| `[02.24]` | Expected unary or binary operator in expression |
| `[02.25]` | Expected } for closing of initialization of array |
| `[02.26]` | Expected ; for the end of statement |
| `[02.27]` | Expected ] for the closing of index of array |
| `[02.28]` | Expected = for the assignment of array |
| `[02.29]` | Expected =(for variable assignment) or [(for array assignment) |
| `[02.30]` | Expected ( for the condition of IF (opening) |
| `[02.31]` | Expected ) for the condition of IF (closing) |
| `[02.32]` | Expected { for the opening of IF block |
| `[02.33]` | Expected if or { for valid if-else block |
| `[02.34]` | ! NOT allowed in post-fix [var] |
| `[02.35]` | Expected ; at end of statement [var] |
| `[02.36]` | ! NOT allowed in post-fix [array] |
| `[02.37]` | Expected ; at end of statement [array] |
| `[02.38]` | Expected lvalue after unary operator |
| `[02.39]` | Expected ( for the condition of WHILE (opening) |
| `[02.40]` | Expected ) for the condition of WHILE (closing) |
| `[02.41]` | Expected { for the opening of WHILE block |
| `[02.42]` | Expected ( for the starting of FOR init |
| `[02.43]` | Expected ; after init |
| `[02.44]` | Expected ; after cond |
| `[02.45]` | Expected ) for the closing of FOR update |
| `[02.46]` | Expected { for starting of for body |

### File 03 Errors (03.01 - 03.28)

| Error Code | Description |
|------------|-------------|
| `[03.01]` | AST is NOT accessible |
| `[03.02]` | Right expr type doesn't match with declared type |
| `[03.03]` | Right expr type doesn't match with left var type |
| `[W \|\| 03.04]` | Expected BOOL in condition of IF but found something else |
| `[W \|\| 03.05]` | Expected BOOL in condition of IF-ELSE but found something else |
| `[W \|\| 03.06]` | Expected BOOL in condition of WHILE but found something else |
| `[W \|\| 03.07]` | Expected BOOL in condition of FOR but found something else |
| `[03.08]` | Expected BOOL after ! |
| `[03.09]` | Expected INT around ++/-- |
| `[03.10]` | AST is NOT accessible |
| `[03.11]` | Index of array MUST be int |
| `[03.12]` | Expected INT/DOUBLE on both sides of binary operator (+ - * /) |
| `[03.13]` | Expected INT/DOUBLE/BOOLEAN on both sides of binary operator (== != > < >= <=) |
| `[W \|\| 03.14]` | Different data type found around binop |
| `[03.15]` | Expected BOOLEAN around binop (&& \|\|) |
| `[03.16]` | Unexpected binop code returned |
| `[03.17]` | Expected BOOLEAN after ! |
| `[03.18]` | Expected INT around ++/-- |
| `[03.19]` | Unexpected unary code returned |
| `[W \|\| 03.20]` | Unexpected = found in expression |
| `[03.21]` | Unknown AST found |
| `[03.22]` | Variable () already exists in current scope |
| `[03.23]` | Index symbol (%s) is undefined |
| `[03.24]` | Index symbol (%s) MUST be of type INT |
| `[W \|\| 03.25]` | Index Symbol (%s) is NOT initialized |
| `[03.26]` | AST is NOT accessible |
| `[03.27]` | ! NOT allowed in update section of FOR |
| `[03.28]` | Expected either INCREMENTS/DECREMENTS/ASSIGNMENTS statements in update section of for |

### File 04 Errors (04.01 - 04.07)

| Error Code | Description |
|------------|-------------|
| `[04.01]` | AST is NOT accessible or NULL |
| `[04.02]` | Memory allocation failed for temporary variable |
| `[04.03]` | Memory allocation failed for label |
| `[04.04]` | Memory allocation failed for three-address code instruction |
| `[04.05]` | Maximum limit of three-address code instructions reached |
| `[04.06]` | Unknown or unsupported AST node type found |
| `[04.07]` | Invalid operator type in AST node |

### File 05 Errors (05.01 - 05.08)

| Error Code | Description |
|------------|-------------|
| `[05.01]` | Memory allocation failed for basic block |
| `[05.02]` | Maximum limit of basic blocks reached |
| `[05.03]` | Memory allocation failed for block properties |
| `[05.04]` | Block index mismatch during property assignment |
| `[05.05]` | Memory allocation failed for address pointer array |
| `[05.06]` | Block not found in block array |
| `[05.07]` | Invalid block structure detected |
| `[05.08]` | CFG formation error - label not found |

### File 06 Errors (06.01 - 06.06)

| Error Code | Description |
|------------|-------------|
| `[06.01]` | Memory allocation failed for assembly instruction |
| `[06.02]` | Assembly buffer is full - maximum instruction limit reached |
| `[06.03]` | Invalid operand type in three-address code |
| `[06.04]` | Unknown instruction type during code generation |
| `[06.05]` | Register allocation failed |
| `[06.06]` | Invalid operation for target code generation |

### File 06_01 Errors (06_01.01 - 06_01.03)

| Error Code | Description |
|------------|-------------|
| `[06_01.01]` | Real assembly buffer is full - maximum instruction limit reached |
| `[06_01.02]` | Memory allocation failed for real assembly instruction |
| `[06_01.03]` | Variable stack map is full - maximum variable limit reached |

---

## Error Format Convention

All errors follow this format:
```
[File#.Error#] -> Description
```

Warnings follow this format:
```
[W || File#.Error#] -> Description
```

### Exit Codes by File
- **File 01**: `exit(1)` - Lexical Analysis errors
- **File 02**: `exit(2)` - Syntax/AST Generation errors
- **File 03**: `exit(3)` - Semantic Analysis errors
- **File 04**: `exit(4)` - Intermediate Code Generation errors
- **File 05**: `exit(5)` - Optimization errors
- **File 06**: `exit(6)` - Target Code Generation errors

---

**Last Updated**: November 23, 2025  
**Compiler**: RM Compiler (Level-2 C Compiler)  
**Author**: Ridham Khurana (RM)
